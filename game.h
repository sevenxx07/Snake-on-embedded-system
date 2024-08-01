/**
 * Module for running the game after setting it 
 * @file game.h
 * @author Sára Veselá (veselsa1@fel.cvut.cz) & Michal Vaverka (vavermic@fel.cvut.cz)
 * @date 2021-27-05
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "utils.h"

void fracSleep(float sec);

void draw_introduction(unsigned int* playing_area, unsigned char* parlcd_mem_base, unsigned char* led_mem_base);
/*
* Structure of game - setting and pointers of display and led
*/
game_t* create_game(unsigned char* parlcd_mem_base, unsigned char* led_mem_base, unsigned int* playing_area);

/*
* Create 4px borders around the display.
*/
void set_borders(unsigned char *parlcd_mem_base, unsigned int* area, int pixel_size);

/*
* Clearing game area -> LCD display
*/
void clear_area(unsigned char* parlcd_mem_base, unsigned int* area);

/*
* Main in-game function, which is being called in the main game loop.
*/
int update_game(game_t *game, playing_board_t *playing_board);

#endif
