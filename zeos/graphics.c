/*
 * graphics.c -
 */
#include <graphics.h>
#include <libc.h>

char* sys_color = " ";
int cooldown = 0;

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

void fill_buff(char* buff, int n);

void erease(int x, int y, int sx, int sy)
{
	char buff[sx];
	fill_buff(buff, sx);
	
	set_color(0x0,0x0);
	for(int i = 0; i < sy; ++i) {
		gotoxy(x,y-i);
		write(1, buff, sx);
	}
}

void init_user()
{
	spaceship.x = (MAX_X-SPSHP_X)/2;
	spaceship.y = MAX_Y-2;
	spaceship.lives = 3;
}

void init_enemies()
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			enemy[i].lives = 1;
			enemy[i].x = 8+j*3*(ENEMY_X+2);
			enemy[i].y = ENEMY_Y*(i+1)+i;
		}
	}
}

void init_beams()
{
	for(int i = 0; i < MAX_LASERS; ++i) {
		laser[i].x = -1;
		laser[i].y = -1;
		laser[i].dir = 0;
	}
}

void init_map()
{
	init_user();
	init_enemies();
	init_beams();
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
	char ins[32] = "PRESS 'I' FOR INSTRUCTIONS";
	int s = strlen(buff);
	int s_ins = strlen(ins);
	
	set_color(0xf, 0x0);
	gotoxy((MAX_X-s)/2, MAX_Y*4/5);
	write(1, buff, s);
	
	gotoxy((MAX_X-s_ins)/2, MAX_Y*4/5+2);
	write(1, ins, s_ins);
	
	char name[32] = "SPACIAL OUTER ADVENTURE";
	s = strlen(name);
	gotoxy((MAX_X-s)/2, MAX_Y*3/5+2);
	write(1, name, s);
	
	set_color(0xc, 0x0);
	gotoxy((MAX_X-s)/2, MAX_Y*3/5+2);
	write(1, "S", 1);
	
	gotoxy((MAX_X-s)/2 + 8, MAX_Y*3/5+2);
	write(1, "O", 1);
	
	gotoxy((MAX_X-s)/2 + 14, MAX_Y*3/5+2);
	write(1, "A", 1);
	
	setColor("red");
	print_assignatura();
	
	//print_spaceship((MAX_X+SPSHP_X)*5/7, (MAX_Y+SPSHP_Y)/2);
}

void display_win()
{
	char msg[20] = "GAME OVER";
	set_color(0xc, 0x0);
	gotoxy((MAX_X-strlen(msg))/2, MAX_Y/2);
	write(1, msg, strlen(msg));
	
	char help[MAX_X] = "PRESS 'R' TO RESTART THE GAME";
	gotoxy((MAX_X-strlen(help))/2, MAX_Y/2+3);
	write(1, help, strlen(help));
}

void display_lose()
{
	char msg[20] = "YOU WIN!";
	set_color(0xc, 0x0);
	gotoxy((MAX_X-strlen(msg))/2, MAX_Y/2);
	write(1, msg, strlen(msg));
	
	char help[MAX_X] = "PRESS 'R' TO RESTART THE GAME";
	gotoxy((MAX_X-strlen(help))/2, MAX_Y/2+3);
	write(1, help, strlen(help));
}

void print_instructions()
{
	char line1[MAX_X] = "T'HAN ENCARREGAT LA MISSIO DE DERROTAR ELS MALIGNES INoDes (IMPOSTORS";
	char line2[MAX_X] = "NO DESITJATS), DERROTA TOTS ELS ENEMICS PER GUANYAR!";
	char line3[MAX_X] = "AJUDA'T DE LES TECLES 'A' i 'D' PER MOURE'T A LA DRETA I ESQUERRA";
	char line4[MAX_X] = "RESPECTIVAMENT, I PER DISPARAR LA TECLA 'S'";
	char line5[MAX_X] = "T'ATREVEIXES?";
	char line6[MAX_X] = "APRETA 'X' PER SORTIR";
	char line[MAX_X] = "#############################################################################";
	
	set_color(0xf, 0x0);
	gotoxy(5, 3);
	write(1, line1, strlen(line1));
	gotoxy(5, 4);
	write(1, line2, strlen(line2));
	gotoxy(5, 6);
	write(1, line3, strlen(line3));
	gotoxy(5, 7);
	write(1, line4, strlen(line4));
	gotoxy(5, 9);
	write(1, line5, strlen(line5));
	
	gotoxy(MAX_X-5-strlen(line6), MAX_Y-1);
	write(1, line6, strlen(line6));
	
	set_color(0x7,0x7);
	gotoxy(1, 11);
	write(1, line, strlen(line));
}

void spawn_attack(int x, int y, int dir, int pos)
{
	set_color(0x7,0x7);
	gotoxy(x,y);
	write(1, "#", 1);
	
	laser[pos].x = x;
	laser[pos].y = y;
	laser[pos].dir = dir;
}

void attack(int x, int y, int user, int pos)
{
	if(user == 0) { //SPACESHIP
		spawn_attack(x+(SPSHP_X/2), y-SPSHP_Y, -1, pos);
	}
	else if(user == 1) { //ENEMY
		spawn_attack(x+(ENEMY_X/2), y+1, 1, pos);
	}
}

int avail_laser()
{
	for(int i = 0; i < MAX_LASERS; ++i) {
		if(laser[i].dir == 0) return i;
	}
	return -1;
}

int use_spaceship(char* k, int x, int y)
{
	if(cooldown > 0) {
		--cooldown;
	}
	
	if(*k == 'a' && x > MIN_X) { //LEFT
		--x;
		erease(x+1, y, SPSHP_X, SPSHP_Y);
		*k = '\0';
	}
	else if(*k == 'd' && x <= MAX_X-SPSHP_X) { //RIGHT
		++x;
		erease(x-1, y, SPSHP_X, SPSHP_Y);
		*k = '\0';
	}
	else if(*k == 's') { //ATTACK
		int pos = avail_laser();
	
		if(cooldown == 0 && pos >= 0) {
			attack(x, y, -1, pos);
			cooldown = 5;
		}
		*k = '\0';
	}
	
	return x;
}

int next_enemy(int i)
{
	int y_laser = laser[i].y+laser[i].dir;
	
		for(int j = 0; j < NUM_ENEMY; ++i) {
		if(laser[i].x >= enemy[j].x && laser[i].x < enemy[j].x+ENEMY_X &&
		   y_laser < enemy[j].y && y_laser >= enemy[j].y-ENEMY_Y) {
			return j;
		}
	}
	return -1;
}

int next_spaceship(int i)
{
	for(int j = 0; j < NUM_ENEMY; ++i) {
		if(laser[i].x >= enemy[j].x && laser[i].x < enemy[j].x+ENEMY_X &&
		   laser[i].y < enemy[j].y && laser[i].y >= enemy[j].y-ENEMY_Y) {
			return j;
		}
	}
	return -1;
}

void remove_heart()
{
	gotoxy(spaceship.lives*3, MAX_Y);
	set_color(0x0, 0x0);
	write(1, "##", 2);
	
	--spaceship.lives;
}

void kill_enemy(int i)
{
	erease(enemy[i].x, enemy[i].y, ENEMY_X, ENEMY_Y);
	
	enemy[i].x = -1;
	enemy[i].y = -1;
}

void continue_laser(int i)
{
	setColor("black");
	gotoxy(laser[i].x, laser[i].y);
	write(1, "#", 1);
	
	laser[i].y += laser[i].dir;
	
	setColor("white");
	gotoxy(laser[i].x, laser[i].y);
	write(1, "#", 1);
}

void delete_laser(int i)
{
	setColor("black");
	gotoxy(laser[i].x, laser[i].y);
	write(1, "#", 1);
	
	laser[i].dir = 0;
}

void move_lasers()
{
	for(int i = 0; i < MAX_LASERS; ++i) {
		if(laser[i].dir < 0) { //The laser is moving towards the enemis
			int found = next_enemy(i);
			
			if(found != -1) {
				kill_enemy(i);
				delete_laser(i);
			}
			else if(laser[i].y > MIN_Y) {
				continue_laser(i);
			}
			else {
				delete_laser(i);
			}
		}
		else if(laser[i].dir > 0) { //The laser is moving towards us
			int found = next_spaceship(i);
			
			if(found) {
				remove_heart();
			}
			else if(laser[i].y < MAX_Y) {
				continue_laser(i);
			}
			else {
				delete_laser(i);
			}
		}
	}
}
