/*
 * keyboard.c - 
 */

#include <keyboard.h>

struct buffer keystrokes;

void init_buffer()
{
	//We initialize the pointers
	keystrokes.p_read = 0;
	keystrokes.p_write = 0;
}

void write_key(char c)
{
	//We check if we have enough space
	if(keystrokes.num_writes < BUFFER_SIZE) {
		//We write at position p_write the character c
		keystrokes.buffer[keystrokes.p_write] = c;
		
		//We increase the writing position
		keystrokes.p_write = (keystrokes.p_write+1)%BUFFER_SIZE;
			
		//We increase the number of writes
		keystrokes.num_writes++;
	}
}

int read_keys(char* b, int maxchars)
{		
	int i;
	
	//We check that the number of read elements is less or equal than the buffer 
	//size and the number of writen elements
	for(i = 0; i < maxchars && i < BUFFER_SIZE && i < keystrokes.num_writes; ++i)
	{
		int pos = (keystrokes.p_read+i)%BUFFER_SIZE;
		*(b+i) = keystrokes.buffer[pos];
	}
	keystrokes.num_writes -= i;
	keystrokes.p_read = (keystrokes.p_read+i)%BUFFER_SIZE;
	return i;	
}
