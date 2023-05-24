/*
 * performance.c - 
 */
#include <performance.h>
#include <io.h>

int num_frames = 0;

void itoa(int a, char* b);

void print_fps()
{
	char fps[30];
	int size = 0;

	//We clear the last value
	for(int i = 0; i < size+4; ++i) { //Last value + " fps"
		printc_xy(i,0,' ');
	}

	//We print the fps
	itoa(num_frames, fps);
	size = printk_xy(0,0,fps);
	printk_xy(size+1,0,"fps");

	//We reset the fps
	num_frames = 0;
}
