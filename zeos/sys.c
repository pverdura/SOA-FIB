/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <p_stats.h>

#include <errno.h>

#include <keyboard.h>

#define LECTURA 0
#define ESCRIPTURA 1

extern Byte x, y;
extern int color;

void * get_ebp();

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -EBADF; 
  if (permissions!=ESCRIPTURA) return -EACCES; 
  return 0;
}

void user_to_system(void)
{
  update_stats(&(current()->p_stats.user_ticks), &(current()->p_stats.elapsed_total_ticks));
}

void system_to_user(void)
{
  update_stats(&(current()->p_stats.system_ticks), &(current()->p_stats.elapsed_total_ticks));
}

int sys_ni_syscall()
{
	return -ENOSYS; 
}

int sys_getpid()
{
	return current()->PID;
}

int global_PID=1000;

int ret_from_fork()
{
  return 0;
}

int sys_fork(void)
{
  struct list_head *lhcurrent = NULL;
  union task_union *uchild;
  
  /* Any free task_struct? */
  if (list_empty(&freequeue)) return -ENOMEM;

  lhcurrent=list_first(&freequeue);
  
  list_del(lhcurrent);
  
  uchild=(union task_union*)list_head_to_task_struct(lhcurrent);
  
  /* Copy the parent's task struct to child's */
  copy_data(current(), uchild, sizeof(union task_union));
  
  /* new pages dir */
  allocate_DIR((struct task_struct*)uchild);
  
  /* Allocate pages for DATA+STACK */
  int new_ph_pag, pag, i;
  page_table_entry *process_PT = get_PT(&uchild->task);
  for (pag=0; pag<NUM_PAG_DATA; pag++)
  {
    new_ph_pag=alloc_frame();
    if (new_ph_pag!=-1) /* One page allocated */
    {
      set_ss_pag(process_PT, PAG_LOG_INIT_DATA+pag, new_ph_pag);
    }
    else /* No more free pages left. Deallocate everything */
    {
      /* Deallocate allocated pages. Up to pag. */
      for (i=0; i<pag; i++)
      {
        free_frame(get_frame(process_PT, PAG_LOG_INIT_DATA+i));
        del_ss_pag(process_PT, PAG_LOG_INIT_DATA+i);
      }
      /* Deallocate task_struct */
      list_add_tail(lhcurrent, &freequeue);
      
      /* Return error */
      return -EAGAIN; 
    }
  }
  
  /******************* Allocate shared frames from the parent *******************/
  page_table_entry *parent_PT = get_PT(current());
  for(pag = NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag < TOTAL_PAGES; ++pag) {
  	
  	/* We check if the page uses a shared frame */
  	if(used_addr((void*)(pag*PAGE_SIZE)) && shm_addr((void*)(pag*PAGE_SIZE))) {
  		int shm_frame = get_frame(parent_PT, pag);
  		set_ss_pag(process_PT, pag, shm_frame);
  		
  		int id = get_shm_id(shm_frame);
  		increment_ref(id);
  	}
  } 
  /******************************************************************************/

  /* Copy parent's SYSTEM and CODE to child. */
  for (pag=0; pag<NUM_PAG_KERNEL; pag++)
  {
    set_ss_pag(process_PT, pag, get_frame(parent_PT, pag));
  }
  for (pag=0; pag<NUM_PAG_CODE; pag++)
  {
    set_ss_pag(process_PT, PAG_LOG_INIT_CODE+pag, get_frame(parent_PT, PAG_LOG_INIT_CODE+pag));
  }
  /* Copy parent's DATA to child. */
  for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE; pag<NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag++)
  {
  	/* We get a temporat page */
    int copy_page = pag+NUM_PAG_DATA;
    
    /* We check it's not used */
  	while(get_frame(parent_PT, copy_page) != FREE_FRAME) {
  		++copy_page;
  	}
  
    /* Map one child page to parent's address space. */
    set_ss_pag(parent_PT, copy_page, get_frame(process_PT, pag));
    copy_data((void*)(pag<<12), (void*)(copy_page<<12), PAGE_SIZE);
    del_ss_pag(parent_PT, copy_page);
    //set_ss_pag(parent_PT, PAG_LOG_INIT_DATA+pag, get_frame(parent_PT, PAG_LOG_INIT_DATA+pag));
  }
  /* Deny access to the child's memory space */
  set_cr3(get_DIR(current()));

  uchild->task.PID=++global_PID;
  uchild->task.state=ST_READY;

  int register_ebp;		/* frame pointer */
  /* Map Parent's ebp to child's stack */
  register_ebp = (int) get_ebp();
  register_ebp=(register_ebp - (int)current()) + (int)(uchild);

  uchild->task.register_esp=register_ebp + sizeof(DWord);

  DWord temp_ebp=*(DWord*)register_ebp;
  /* Prepare child stack for context switch */
  uchild->task.register_esp-=sizeof(DWord);
  *(DWord*)(uchild->task.register_esp)=(DWord)&ret_from_fork;
  uchild->task.register_esp-=sizeof(DWord);
  *(DWord*)(uchild->task.register_esp)=temp_ebp;

  /* Set stats to 0 */
  init_stats(&(uchild->task.p_stats));

  /* Queue child process into readyqueue */
  uchild->task.state=ST_READY;
  list_add_tail(&(uchild->task.list), &readyqueue);
  
  return uchild->task.PID;
}

#define TAM_BUFFER 512

int sys_write(int fd, char *buffer, int nbytes) {
char localbuffer [TAM_BUFFER];
int bytes_left;
int ret;

	if ((ret = check_fd(fd, ESCRIPTURA)))
		return ret;
	if (nbytes < 0)
		return -EINVAL;
	if (!access_ok(VERIFY_READ, buffer, nbytes))
		return -EFAULT;
	
	bytes_left = nbytes;
	while (bytes_left > TAM_BUFFER) {
		copy_from_user(buffer, localbuffer, TAM_BUFFER);
		ret = sys_write_console(localbuffer, TAM_BUFFER);
		bytes_left-=ret;
		buffer+=ret;
	}
	if (bytes_left > 0) {
		copy_from_user(buffer, localbuffer,bytes_left);
		ret = sys_write_console(localbuffer, bytes_left);
		bytes_left-=ret;
	}
	return (nbytes-bytes_left);
}


extern int zeos_ticks;

int sys_gettime()
{
  return zeos_ticks;
}

void sys_exit()
{  
  int i;

  page_table_entry *process_PT = get_PT(current());

  // Deallocate all the propietary physical pages
  for (i=0; i<NUM_PAG_DATA; i++)
  {
    free_frame(get_frame(process_PT, PAG_LOG_INIT_DATA+i));
    del_ss_pag(process_PT, PAG_LOG_INIT_DATA+i);
  }
  /************************** Deallocate shared frames **************************/
  for (int pag=PAG_LOG_INIT_DATA+NUM_PAG_DATA; pag<TOTAL_PAGES; ++pag)
  {
  	if(used_addr((void*)(pag*PAGE_SIZE)) && shm_addr((void*)(pag*PAGE_SIZE))) {
  		int shm_frame = get_frame(get_PT(current()),pag);
  		decrement_ref(shm_frame);
  	}
  }
  /******************************************************************************/
  
  /* Free task_struct */
  list_add_tail(&(current()->list), &freequeue);
  
  current()->PID=-1;
  
  /* Restarts execution of the next process */
  sched_next_rr();
}

/* System call to force a task switch */
int sys_yield()
{
  force_task_switch();
  return 0;
}

extern int remaining_quantum;

int sys_get_stats(int pid, struct stats *st)
{
  int i;
  
  if (!access_ok(VERIFY_WRITE, st, sizeof(struct stats))) return -EFAULT; 
  
  if (pid<0) return -EINVAL;
  for (i=0; i<NR_TASKS; i++)
  {
    if (task[i].task.PID==pid)
    {
      task[i].task.p_stats.remaining_ticks=remaining_quantum;
      copy_to_user(&(task[i].task.p_stats), st, sizeof(struct stats));
      return 0;
    }
  }
  return -ESRCH; /*ESRCH */
}

int sys_read(char *b, int maxchars)
{
	//Parameter checking
	if(maxchars < 0) {
		return -EINVAL;
	}
	if(!access_ok(VERIFY_WRITE, b, sizeof(char))) {
		return -EFAULT;
	}

	//We read the buffer
	return read_keys(b, maxchars);
}

int sys_gotoxy(int px, int py)
{
	//Parameter checking
	if(px >= NUM_COLUMNS || py >= NUM_ROWS || px < 0 || py < 0) {
		return -EINVAL;
	}
	
	//We go to the position (x,y) of the screen
	x = px;
	y = py;
	
	return 0;
}

int sys_set_color(int fg, int bg)
{
	//Parameter checking
	if(fg > 0xf || bg > 0xf || fg < 0 || bg < 0) {
		return -EINVAL;
	} 
	
	//We change the foreground to fg color and the background to bg color
	color = (fg * 0x100)+(bg * 0x1000);
	
	return 0;
}

void* sys_shmat(int id, void* addr)
{
	//Parameter checking
	if(id < 0 || id > NR_SHARED_FRAMES) {
		return (void*)(-EINVAL);
	}
	if(addr == NULL || usr_addr_ok(addr)) {
		
		//We check if we have to get a new address
		if(addr == NULL || used_addr(addr)) {
			//We get an empty usr addr
			int new_addr = get_empty_addr();
			
			//We check if there was empty pages
			if(new_addr < 0) {
				return (void*)(-ENOMEM);
			}
			addr = (void*)new_addr;
		}
		//We get the desired frame
		int frame = get_shm_frame(id);
		
		//We map the address to the frame
		set_ss_pag(get_PT(current()), (int)addr/PAGE_SIZE, frame);
		increment_ref(id);
	}
	else {
		return (void*)(-EFAULT);
	}
	return addr;
}

int sys_shmdt(void* addr)
{
	//Parameter checking
	if(addr == NULL || !shm_addr(addr)) {
		return -EFAULT;
	}
	
	int pag = (int)addr/PAGE_SIZE;
	int frame = get_frame(get_PT(current()), pag);
	
	//We unmap the frame to the address addr
	del_ss_pag(get_PT(current()), pag);
	decrement_ref(frame);
	set_cr3(get_DIR(current()));	//We flush the TLB
	
	return 0;
}

int sys_shmrm(int id)
{
	//Parameter checking
	if(id < 0 || id > NR_SHARED_FRAMES) {
		return -EINVAL;
	}
	
	//We mark the frame to be cleared (set all to '0')
	mark_frame(id);
	return 0;
}
