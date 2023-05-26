/*
 * graphics.h - Design of game characters
 */
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define MIN_X 0
#define MIN_Y 0
#define MAX_X 79
#define MAX_Y 24

#define SPSHP_X 7
#define SPSHP_Y 3
void print_spaceship(int x, int y);

#define ENEMY_X 5
#define ENEMY_Y 2
void print_enemy_1(int x, int y);
void print_enemy_2(int x, int y);
void print_enemy_3(int x, int y);
int* enemy_size();

#define BOSS_X 24
#define BOSS_Y 5
void print_boss(int x, int y);

void print_heart(int x, int y);

void clear_screen();
void erase(int x, int y, int sx, int sy);

void init_map();
void display_menu();
void print_instructions();

int use_spaceship(char* k, int x, int y);

#endif /* __GRAPHICS_H__*/
