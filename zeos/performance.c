/*
 * performance.c - 
 */
#include <performance.h>
#include <libc.h>

int num_frames = 0;
char fps[10];
int size = 0;
int last_ticks = 0;

/* Fills the buffer with the '#' character */
void fill_buff(char* buff, int n)
{
	for(int i = 0; i < n; ++i) {
		buff[i] = '#';
	}
}

/* Adds " fps" at the end of the buffer 'buff' */
int add_fps(char* buff)
{
	int n = strlen(buff);
	
	buff[n] = ' ';
	buff[n+1] = 'f';
	buff[n+2] = 'p';
	buff[n+3] = 's';	
	
	return n+4;
}

void print_fps()
{
	int current_ticks = gettime();
	
	if((current_ticks - last_ticks) >= TICKS_SECOND) {
		int new_size; //How many bytes we have to print

		itoa(num_frames, fps);
		new_size = add_fps(fps);
		
		if(new_size < size) {
			int n = size-new_size;
			char buff[n];
			
			fill_buff(buff, n);
			
			//font: black, background: black
			set_color(0x0, 0x0);
			
			//We clean the last value
			gotoxy(n,0);
			write(1, buff, n);
		}
		
		//font: white, background: black
		set_color(0x7, 0x0);
		
		//We print the fps at left-upper corner
		gotoxy(0, 0);
		write(1, fps, strlen(fps));

		//We reset the fps
		num_frames = 0;
		
		//We update the number of writen bytes and the last ticks
		size = new_size;
		last_ticks = current_ticks;
	}
}

void inc_fps()
{
	num_frames++;
}
