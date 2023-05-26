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
					int x = (MAX_X-SPSHP_X)/2;
					int y = (MAX_Y-5);
					menu = 0;
					print_instructions();
					init_map();
							
					while(tecla[0] != 'x') {
						print_fps();
						print_spaceship(x, y);
						
						move_lasers();
						x = use_spaceship(&tecla[0], x, y);
						inc_fps();
					}
				}
			}
			clear_screen();
			init_map();
			int win = 0;
			int num_enem = NUM_ENEMY;
			int lastclock = 0;
			
			for(int i = 0; i < spaceship.lives; ++i) {
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
			
			while(spaceship.lives > 0 && !win) {
				print_fps();
				print_spaceship(spaceship.x,spaceship.y);
				
				if(num_enem == 0) win = 1;
				
				if(gettime()-lastclock > TICKS_SECOND*3) {
					move_lasers();
					lastclock = gettime();
				}
				spaceship.x = use_spaceship(&tecla[0], spaceship.x, spaceship.y);
				inc_fps();
			}
			
			int display = 0;
			while(win && tecla[0] != 'r') { //WIN
				print_fps();
				
				if(!display) {
					display_win();
					display = 1;
				}
			}
			while(!win && tecla[0] != 'r') { //LOSE
				print_fps();
				
				if(!display) {
					display_lose();
					display = 1;
				}
			}
			//We reset the variables
			play = 0;
			menu = 0;	
		}
	}
}
