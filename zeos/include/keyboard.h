/*
 * keyboard.h - Structures and macros for keyboard interruptions
 */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define BUFFER_SIZE 15

struct buffer{
	char buffer[BUFFER_SIZE];
	int p_read;
	int p_write;
	int num_writes;
};

/* Initializes the buffer where we copy the pressed keys */
void init_buffer();

/* Adds the pressed key to the copy buffer */
void write_key(char c);

/* Reads the lasts maxchars writen keys to b */
int read_keys(char* b, int maxchars);

#endif /* __KEYBOARD_H__ */
