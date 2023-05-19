/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <types.h>
#include <hardware.h>
#include <segment.h>
#include <sched.h>
#include <mm.h>
#include <io.h>
#include <utils.h>
#include <p_stats.h>

/**
 * Container for the Task array and 2 additional pages (the first and the last one)
 * to protect against out of bound accesses.
 */
union task_union protected_tasks[NR_TASKS+2]
  __attribute__((__section__(".data.task")));

union task_union *task = &protected_tasks[1]; /* == union task_union task[NR_TASKS] */

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;

// Free task structs
struct list_head freequeue;
// Ready queue
struct list_head readyqueue;

void init_stats(struct stats *s)
{
	s->user_ticks = 0;
	s->system_ticks = 0;
	s->blocked_ticks = 0;
	s->ready_ticks = 0;
	s->elapsed_total_ticks = get_ticks();
	s->total_trans = 0;
	s->remaining_ticks = get_ticks();
}

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

#define DEFAULT_QUANTUM 10

int remaining_quantum=0;

int get_quantum(struct task_struct *t)
{
  return t->total_quantum;
}

void set_quantum(struct task_struct *t, int new_quantum)
{
  t->total_quantum=new_quantum;
}

struct task_struct *idle_task=NULL;

void update_sched_data_rr(void)
{
  remaining_quantum--;
}

int needs_sched_rr(void)
{
  if ((remaining_quantum==0)&&(!list_empty(&readyqueue))) return 1;
  if (remaining_quantum==0) remaining_quantum=get_quantum(current());
  return 0;
}

void update_process_state_rr(struct task_struct *t, struct list_head *dst_queue)
{
  if (t->state!=ST_RUN) list_del(&(t->list));
  if (dst_queue!=NULL)
  {
    list_add_tail(&(t->list), dst_queue);
    if (dst_queue!=&readyqueue) t->state=ST_BLOCKED;
    else
    {
      update_stats(&(t->p_stats.system_ticks), &(t->p_stats.elapsed_total_ticks));
      t->state=ST_READY;
    }
  }
  else t->state=ST_RUN;
}

void sched_next_rr(void)
{
  struct list_head *e;
  struct task_struct *t;

  if (!list_empty(&readyqueue)) {
	e = list_first(&readyqueue);
    list_del(e);

    t=list_head_to_task_struct(e);
  }
  else
    t=idle_task;

  t->state=ST_RUN;
  remaining_quantum=get_quantum(t);

  update_stats(&(current()->p_stats.system_ticks), &(current()->p_stats.elapsed_total_ticks));
  update_stats(&(t->p_stats.ready_ticks), &(t->p_stats.elapsed_total_ticks));
  t->p_stats.total_trans++;

  task_switch((union task_union*)t);
}

void schedule()
{
  update_sched_data_rr();
  if (needs_sched_rr())
  {
    update_process_state_rr(current(), &readyqueue);
    sched_next_rr();
  }
}

void init_idle (void)
{
  struct list_head *l = list_first(&freequeue);
  list_del(l);
  struct task_struct *c = list_head_to_task_struct(l);
  union task_union *uc = (union task_union*)c;

  c->PID=0;

  c->total_quantum=DEFAULT_QUANTUM;

  init_stats(&c->p_stats);

  allocate_DIR(c);

  uc->stack[KERNEL_STACK_SIZE-1]=(unsigned long)&cpu_idle; /* Return address */
  uc->stack[KERNEL_STACK_SIZE-2]=0; /* register ebp */

  c->register_esp=(int)&(uc->stack[KERNEL_STACK_SIZE-2]); /* top of the stack */

  idle_task=c;
}

void setMSR(unsigned long msr_number, unsigned long high, unsigned long low);

void init_task1(void)
{
  struct list_head *l = list_first(&freequeue);
  list_del(l);
  struct task_struct *c = list_head_to_task_struct(l);
  union task_union *uc = (union task_union*)c;

  c->PID=1;

  c->total_quantum=DEFAULT_QUANTUM;

  c->state=ST_RUN;

  remaining_quantum=c->total_quantum;

  init_stats(&c->p_stats);

  allocate_DIR(c);

  set_user_pages(c);

  tss.esp0=(DWord)&(uc->stack[KERNEL_STACK_SIZE]);
  setMSR(0x175, 0, (unsigned long)&(uc->stack[KERNEL_STACK_SIZE]));

  set_cr3(c->dir_pages_baseAddr);
}

void init_freequeue()
{
  int i;

  INIT_LIST_HEAD(&freequeue);

  /* Insert all task structs in the freequeue */
  for (i=0; i<NR_TASKS; i++)
  {
    task[i].task.PID=-1;
    list_add_tail(&(task[i].task.list), &freequeue);
  }
}

void init_sched()
{
  init_freequeue();
  INIT_LIST_HEAD(&readyqueue);
}

struct task_struct* current()
{
  int ret_value;
  
  return (struct task_struct*)( ((unsigned int)&ret_value) & 0xfffff000);
}

struct task_struct* list_head_to_task_struct(struct list_head *l)
{
  return (struct task_struct*)((int)l&0xfffff000);
}

/* Do the magic of a task switch */
void inner_task_switch(union task_union *new)
{
  page_table_entry *new_DIR = get_DIR(&new->task);

  /* Update TSS and MSR to make it point to the new stack */
  tss.esp0=(int)&(new->stack[KERNEL_STACK_SIZE]);
  setMSR(0x175, 0, (unsigned long)&(new->stack[KERNEL_STACK_SIZE]));

  /* TLB flush. New address space */
  set_cr3(new_DIR);

  switch_stack(&current()->register_esp, new->task.register_esp);
}


/* Force a task switch assuming that the scheduler does not work with priorities */
void force_task_switch()
{
  update_process_state_rr(current(), &readyqueue);

  sched_next_rr();
}


/* Frames that will be used to share memory space between processes */
struct {
	int frame;
	int num_refs;
	int clean_mark;
} shm_frames[NR_SHARED_FRAMES];

/* Initializes the frames */
void init_shm_frames()
{
	for(int i = 0; i < NR_SHARED_FRAMES; ++i) {
		shm_frames[i].frame = alloc_frame();
		shm_frames[i].num_refs = 0;
		shm_frames[i].clean_mark = 0;
	}
}

/* Returns the frame from the identifier */
int get_shm_frame(int id)
{
	return shm_frames[id].frame;
}

/* Increments the number of references of the frame id */
void ref_shm_frame(int id)
{
	shm_frames[id].num_refs++;
}

/* Set the whole frame content to '0' if possible */
void clean_frame(int id)
{
	int* addr = (int*)shm_frames[id].frame; 
		
	//We clean the frame (set to 0)
	for(int i = 0; i < PAGE_SIZE; ++i) {
		addr[i] = 0;
	}
					
	//We remove the mark
	shm_frames[id].clean_mark = 0;
}

/* Decrements the number of references if the frame */
void deref_shm_frame(int frame)
{
	for(int id = 0; id < NR_SHARED_FRAMES; ++id) {
		if(frame == shm_frames[id].frame) {
			shm_frames[id].num_refs--;
			
			//We clean the frame if it's possible
			if(shm_frames[id].clean_mark && shm_frames[id].num_refs == 0) {
				clean_frame(id);
			}
			return;
		}
	}
}

/* Returns the number of references the frame id has */
int num_refs_shm(int id)
{
	return shm_frames[id].num_refs;
}

/* Checks if the address addr is used */
int used_addr(void* addr)
{
	page_table_entry* PT = get_PT(current());
	return get_frame(PT, (int)addr/PAGE_SIZE);
}

/* Gets an empty address page aligned */
int get_empty_addr()
{
	page_table_entry* PT = get_PT(current());
	
	//We look for an empty page
	for(int i = PAG_LOG_INIT_DATA+NUM_PAG_DATA; i < TOTAL_PAGES; ++i) {
		if(PT[i].entry == 0) {	
			return i*PAGE_SIZE;
		}
	}
	
	//There's no free addres, so we return an error
	return -1;
}

/* Checks if the address is from a shared memory space */
int shm_addr(void* addr)
{
	int frame = get_frame(get_PT(current()),(int)addr/PAGE_SIZE);
	
	for(int i = 0; i < NR_SHARED_FRAMES; ++i) {
		if(frame == shm_frames[i].frame) return 1;
	}
	return 0;
}

/* Marks the frame to be cleared when there is no mapped entries */
void mark_frame(int id)
{
	shm_frames[id].clean_mark = 1;
	if(shm_frames[id].num_refs == 0) {
		clean_frame(id);
	}
}
