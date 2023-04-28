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
	//We write at position p_write the character c
	keystrokes.buffer[keystrokes.p_write] = c;
	
	//We increase the writing position
	keystrokes.p_write = (keystrokes.p_write+1)%BUFFER_SIZE;
	
	//We increase the reading position (if needed)
	if(keystrokes.num_writes >= BUFFER_SIZE) {
		keystrokes.p_read = (keystrokes.p_read+1)%BUFFER_SIZE;
	}
	
	//We increase the number of writes
	keystrokes.num_writes++;
}

int read_keys(char* b, int maxchars)
{		
	//We check that maxchars is less than the buffer size
	if(maxchars > BUFFER_SIZE ||
	   maxchars < 0 ||
	   maxchars > keystrokes.num_writes) return -1;
	
	// We copy the last keys to b
	for(int i = 0; i < maxchars; ++i)
	{
		int pos = (keystrokes.p_read+i)%BUFFER_SIZE;
		*(b+pos) = keystrokes.buffer[keystrokes.p_read];
	}
	
	return 0;	
}
