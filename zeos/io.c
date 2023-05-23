/*
 * io.c - 
 */

#include <io.h>

#include <types.h>

/**************/
/** Screen  ***/
/**************/

Byte x, y=19;
int color = 0x0200;

/* Read a byte from 'port' */
Byte inb (unsigned short port)
{
  Byte v;

  __asm__ __volatile__ ("inb %w1,%0":"=a" (v):"Nd" (port));
  return v;
}

void new_line()
{
  x = 0;
  if(y+1 >= NUM_ROWS) {
    int i, j;
    Word *screen = (Word *)0xb8000;
  
    for(i = 0; i < 25; ++i) { // Files (menys la última)
      for(j = 0; j < 80; ++j) { // Columnes
        screen[(i * NUM_COLUMNS + j)] = screen[((i+1) * NUM_COLUMNS + j)]; 
      }
    }
    for(j = 0; j < 80; ++j) {
      screen[(24 * NUM_COLUMNS + j)] = (Word) 0x0;
    }
  } 
  else {
    y+=1;
  }	
}


void printc(char c)
{
     __asm__ __volatile__ ( "movb %0, %%al; outb $0xe9" ::"a"(c)); /* Magic BOCHS debug: writes 'c' to port 0xe9 */
  if (c=='\n')
  {
    new_line();
  }
  else
  {
    Word ch = (Word) (c & 0x0000FF) | (Word) (color);
	Word *screen = (Word *)0xb8000;
	screen[(y * NUM_COLUMNS + x)] = ch;
    if (++x >= NUM_COLUMNS)
    {
      new_line();
    }
  }
}

void printc_xy(Byte mx, Byte my, char c)
{
  Byte cx, cy;
  cx=x;
  cy=y;
  x=mx;
  y=my;
  printc(c);
  x=cx;
  y=cy;
}

void printk(char *string)
{
  int i;
  for (i = 0; string[i]; i++)
    printc(string[i]);
}

int printk_xy(Byte mx, Byte my, char *string)
{
	int size = 0;
	Byte cx, cy;
	cx = x;
	cy = y;
	x = mx;
	y = my;
	
	for(int i = 0; string[i]; ++i) {
		printc(string[i]);
		++size;
	}
	
	x = cx;
	y = cy;
	return size;
}
