#include <libc.h>
#include <performance.h>
#include <graphics.h>

char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
     
     char* tecla;
 	 tecla = (char*)shmat(0,0x0);
     
     int ret = fork();
     
     if(ret == 0) { // Reads the keyboard
     	while(1) {
     		read(tecla, 1);
     	}
     }
     else {	//Joc
     	int play = 0;
     	int menu = 0;
		
		while(1)
		{ 
			clear_screen();
			if(menu == 0) { 
				display_menu();
				menu = 1;
			}
			while(play == 0) { // We are waiting the user to start the game
				print_fps();
				if(tecla[0] == 'a') {
					play = 1;
					tecla[0] = '\0';
				}
			}
			clear_screen();
			
			int x = (MAX_X-SPSHP_X)/2;
			int y = MAX_Y-2;
			int lives = 3;
			
			for(int i = 0; i < lives; ++i) {
				print_heart(i*3,MAX_Y);
			}
			
			while(lives > 0) {
				print_fps();
				print_spaceship(x,y);
					
				inc_fps();
			}
			//We reset the variables
			play = 0;
			menu = 0;	
		}
	}
}
