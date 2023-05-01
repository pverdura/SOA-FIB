#include <libc.h>

char buff[24];

int pid;

void setColor(char* color)
{
	int color_hex;
	
	//We check that if the color we need is already set
	if(color == (char*)"red") color_hex = 0x4;
		else if(color == (char*)"green") color_hex = 0x2;
		else if(color == (char*)"blue") color_hex = 0x1;
		else if(color == (char*)"magenta") color_hex = 0x5;
		else if(color == (char*)"cyan") color_hex = 0x3;
		else if(color == (char*)"orange") color_hex = 0x6;
		else if(color == (char*)"white") color_hex = 0x7;
		else color_hex = 0x0; //"black"
		
		set_color(color_hex, color_hex);
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
     
  while(1) { }
}
