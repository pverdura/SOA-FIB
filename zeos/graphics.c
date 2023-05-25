/*
 * graphics.c -
 */
#include <graphics.h>
#include <libc.h>

char* sys_color = " ";

void setColor(char* color)
{
	int color_hex;
	
	//We check that if the color we need is already set
	if(color != sys_color || sys_color == (char*)" ")
	{
		if(color == (char*)"red") color_hex = 0x4;
		else if(color == (char*)"green") color_hex = 0x2;
		else if(color == (char*)"blue") color_hex = 0x1;
		else if(color == (char*)"magenta") color_hex = 0x5;
		else if(color == (char*)"cyan") color_hex = 0x3;
		else if(color == (char*)"orange") color_hex = 0x6;
		else if(color == (char*)"white") color_hex = 0x7;
		else color_hex = 0x0; //"black"
		
		set_color(color_hex,color_hex);
		sys_color = color;
	}
}

/*   ###
 *  #####
 * # # # #
 */
void print_spaceship(int x, int y)
{
	
	setColor("red");
	//  ***
	// *****
	//# * * #
	gotoxy(x,y);
	write(1, "#", strlen("#"));
	gotoxy(x+6,y);
	write(1, "#", strlen("#"));
	
	setColor("white");
	//  ###
	// #***#
	//* # # *
	gotoxy(x+2,y);
	write(1, "#", strlen("#"));
	gotoxy(x+4,y);
	write(1, "#", strlen("#"));
	gotoxy(x+1,y-1);
	write(1, "#", strlen("#"));
	gotoxy(x+5,y-1);
	write(1, "#", strlen("#"));
	gotoxy(x+2,y-2);
	write(1, "###", strlen("###"));
	
	setColor("cyan");
	//  ***
	// *###*
	//* * * *
	gotoxy(x+2,y-1);
	write(1, "###", strlen("###"));
}

/* # # #
 *  ###
 */
void print_enemy_1(int x, int y)
{
	setColor("orange");
	//* # *
	// #*#
	gotoxy(x+2,y-1);
	write(1, "#", strlen("#"));
	gotoxy(x+1,y);
	write(1, "###", strlen("###"));
	
	setColor("magenta");
	//# * #
	// *#*
	gotoxy(x,y-1);
	write(1, "#", strlen("#"));
	gotoxy(x+4,y-1);
	write(1, "#", strlen("#"));
	gotoxy(x+2,y);
	write(1, "#", strlen("#"));
}

/*  ###
 * #####
 */
void print_enemy_2(int x, int y)
{
	setColor("white");
	// #*#
	//#*#*#
	gotoxy(x+1,y-1);
	write(1, "###", strlen("###"));
	gotoxy(x,y);
	write(1, "#####", strlen("#####"));
	
	setColor("green");
	// *#*
	//*#*#*
	gotoxy(x+2,y-1);
	write(1, "#", strlen("#"));
	gotoxy(x+1,y);
	write(1, "#", strlen("#"));
	gotoxy(x+3,y);
	write(1, "#", strlen("#"));
}

//#####
// # #
void print_enemy_3(int x, int y)
{
	setColor("cyan");
	//#*#*#
	// # #
	gotoxy(x,y-1);
	write(1, "#####", strlen("#####"));
	gotoxy(x+1,y);
	write(1, "#", strlen("#"));
	gotoxy(x+3,y);
	write(1, "#", strlen("#"));
	
	setColor("blue");
	//*#*#*
	// * *
	gotoxy(x+1,y-1);
	write(1, "#", strlen("#"));
	gotoxy(x+3,y-1);
	write(1, "#", strlen("#"));
}


//   ######
// ##########
//#####  #####
// ##      ##
//   ##  ##    (12,5)
void print_boss(int x, int y)
{
	setColor("orange");
	//   ******
	// ##**##**##
	//###**  **###
	// **      **
	//   **  **
	gotoxy(x,y-2);
	write(1, "###", strlen("###"));
	gotoxy(x+9,y-2);
	write(1, "###", strlen("###"));
	gotoxy(x+1,y-3);
	write(1, "##########", strlen("##########"));

	setColor("green");
	//   ######
	// **##**##**
	//*****  *****
	// ##      ##
	//   **  **
	gotoxy(x+3,y-4);
	write(1, "######", strlen("######"));
	gotoxy(x+3,y-3);
	write(1, "##", strlen("##"));
	gotoxy(x+7,y-3);
	write(1, "##", strlen("##"));
	gotoxy(x+1,y-1);
	write(1, "##", strlen("##"));
	gotoxy(x+9,y-1);
	write(1, "##", strlen("##"));

	setColor("red");
	//   ******
	// **********
	//***##  ##***
	// **      **
	//   ##  ##
	gotoxy(x+3,y-2);
	write(1, "##", strlen("##"));
	gotoxy(x+7,y-2);
	write(1, "##", strlen("##"));
	gotoxy(x+3,y);
	write(1, "##", strlen("##"));
	gotoxy(x+7,y);
	write(1, "##", strlen("##"));
}

// ##
//####
//######
//####
// ##    (6,5)
void print_rocket_L(int x, int y)
{
	setColor("cyan");
	// **
	//####
	//#**###
	//####
	// **
	gotoxy(x,y-3);
	write(1, "####", strlen("####"));
	gotoxy(x,y-2);
	write(1, "######", strlen("######"));
	gotoxy(x,y-1);
	write(1, "####", strlen("####"));
	
	setColor("magenta");
	// ##
	//****
	//*##***
	//****
	// ##
	gotoxy(x+1,y-4);
	write(1, "##", strlen("##"));
	gotoxy(x+1,y-2);
	write(1, "##", strlen("##"));
	gotoxy(x+1,y);
	write(1, "##", strlen("##"));
}

//   ##
//  ####
//######
//  ####
//   ##  (6,5)
void print_rocket_R(int x, int y)
{
	setColor("cyan");
	//   **
	//  ####
	//###**#
	//  ####
	//   **
	gotoxy(x+2,y-3);
	write(1, "####", strlen("####"));
	gotoxy(x,y-2);
	write(1, "######", strlen("######"));
	gotoxy(x+2,y-1);
	write(1, "####", strlen("####"));
	
	setColor("magenta");
	//   ##
	//  ****
	//***##*
	//  ****
	//   ##
	gotoxy(x+3,y-4);
	write(1, "##", strlen("##"));
	gotoxy(x+3,y-2);
	write(1, "##", strlen("##"));
	gotoxy(x+3,y);
	write(1, "##", strlen("##"));
}

// <3
void print_heart(int x, int y)
{
	set_color(0xc,0x0);
	gotoxy(x,y);
	write(1, "<3", strlen("<3"));
}

void clear_screen()
{
	setColor("black");
	gotoxy(MAX_X, MAX_Y);
	char buff[28] = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; 
	write(1, buff, strlen(buff));
}

/*    ####
 *   #
 *    ###
 *       #
 *   ####
  */
void print_S()
{
	int aux_x = (MAX_X-17)/2;
	int aux_y = MAX_Y/3;
	
	gotoxy(aux_x+1, aux_y);
	write(1, "####", strlen("####"));
	gotoxy(aux_x, aux_y+1);
	write(1, "#", strlen("#"));
	gotoxy(aux_x+1, aux_y+2);
	write(1, "###", strlen("###"));
	gotoxy(aux_x+4, aux_y+3);
	write(1, "#", strlen("#"));
	gotoxy(aux_x, aux_y+4);
	write(1, "####", strlen("####"));
}

/*    ###
 *   #   #
 *   #   #
 *   #   #
 *    ###
 */
void print_O()
{
	int aux_x = (MAX_X-17)/2+6;
	int aux_y = MAX_Y/3;
	
	gotoxy(aux_x+1, aux_y);
	write(1, "###", strlen("###"));
	gotoxy(aux_x, aux_y+1);
	write(1, "#", strlen("#"));
	gotoxy(aux_x+4, aux_y+1);
	write(1, "#", strlen("#"));
	gotoxy(aux_x, aux_y+2);
	write(1, "#", strlen("#"));
	gotoxy(aux_x+4, aux_y+2);
	write(1, "#", strlen("#"));
	gotoxy(aux_x, aux_y+3);
	write(1, "#", strlen("#"));
	gotoxy(aux_x+4, aux_y+3);
	write(1, "#", strlen("#"));
	gotoxy(aux_x+1, aux_y+4);
	write(1, "###", strlen("###"));
}

/*    ###
 *   #   #
 *   #####
 *   #   #
 *   #   #
 */
void print_A()
{
	int aux_x = (MAX_X-17)/2+12;
	int aux_y = MAX_Y/3;
	
	gotoxy(aux_x+1, aux_y);
	write(1, "###", strlen("###"));
	gotoxy(aux_x, aux_y+1);
	write(1, "#", strlen("#"));
	gotoxy(aux_x+4, aux_y+1);
	write(1, "#", strlen("#"));
	gotoxy(aux_x, aux_y+2);
	write(1, "#####", strlen("#####"));
	gotoxy(aux_x, aux_y+3);
	write(1, "#", strlen("#"));
	gotoxy(aux_x+4, aux_y+3);
	write(1, "#", strlen("#"));
	gotoxy(aux_x, aux_y+4);
	write(1, "#", strlen("#"));
	gotoxy(aux_x+4, aux_y+4);
	write(1, "#", strlen("#"));
}

void print_assignatura()
{
	print_S();
	print_O();
	print_A();
}

void display_menu()
{
	char buff[32] = "PRESS 'A' TO START";
	int s = strlen(buff);
	
	set_color(0xf, 0x8);
	gotoxy((MAX_X-s)/2, MAX_Y*4/5);
	write(1, buff, s);
	
	set_color(0xf, 0x0);
	char name[32] = "SUBESPACIAL OUTER ADVENTURE";
	s = strlen(name);
	gotoxy((MAX_X-s)/2, MAX_Y*3/5+2);
	write(1, name, s);
	
	set_color(0xc, 0x0);
	gotoxy((MAX_X-s)/2, MAX_Y*3/5+2);
	write(1, "S", 1);
	
	gotoxy((MAX_X-s)/2 + 12, MAX_Y*3/5+2);
	write(1, "O", 1);
	
	gotoxy((MAX_X-s)/2 + 18, MAX_Y*3/5+2);
	write(1, "A", 1);
	
	setColor("red");
	print_assignatura();
}
