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
			while(play == 0) { // We are waiting the user to start the game
				if(menu == 0) { 
					clear_screen();
					display_menu();
					menu = 1;
				}
				print_fps();
				if(tecla[0] == 'a') {
					play = 1;
					tecla[0] = '\0';
				}
				else if(tecla[0] == 'i') {
					clear_screen();
					
					while(tecla[0] != 'x') {
						print_fps();
						if(menu != 0) {
							print_instructions();
							menu = 0;
						}
						print_spaceship((MAX_X-SPSHP_X)/2, MAX_Y-5);
						inc_fps();
					}
				}
			}
			clear_screen();
			
			int x = (MAX_X-SPSHP_X)/2;
			int y = MAX_Y-2;
			int lives = 3, num_enem = 27;
			int final = 0;
			
			for(int i = 0; i < lives; ++i) {
				print_heart(i*3,MAX_Y);
			}
			
			for(int i = 0; i < 3; ++i) {
				for(int j = 0; j < 3; ++j) {
					int x_en = 8+j*3*(ENEMY_X+2);
					int y_en = ENEMY_Y*(i+1)+i;
					
					print_enemy_1(x_en, y_en);
					print_enemy_2(x_en+ENEMY_X+2, y_en);
					print_enemy_3(x_en+2*(ENEMY_X+2), y_en);
				}			
			}
			int map[(MAX_X+1)*(MAX_Y+1)];
			
			while(lives > 0 || !final) {
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
