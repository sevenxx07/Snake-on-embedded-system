/**
 * Module of inner representation & functions of playing board (visualisation & game)
 * 
 * @file playing_board.h
 * @author Michal Vaverka (vavermic@fel.cvut.cz)
 * @date 2021-31-05
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef PLAYING_BOARD_H
#define PLAYING_BOARD_H

#include <stdbool.h>
#include <pthread.h>

#include "utils.h"

#define SPOT_DIMENSION 20
#define SPOT_COUNT 384
#define BOARD_HEIGHT 16
#define BOARD_WIDTH 24

#define DIR_UP -24
#define DIR_DOWN 24
#define DIR_LEFT -1
#define DIR_RIGHT 1

/*
* Allocates memory and sets default values for new spot_t variable.
*/
void innit_spot(spot_t *spot, int index);

/*
* Allocates memory and calculates values for new playing_board_t variable. OR CONSTANT??
*/
playing_board_t *create_board(game_t *game, pthread_mutex_t *mtx_c, thread_data_t *thread_data);

/*
* Redraws playing board together with SNAKES and SCORE POINTS.
*/
void draw_playing_board(playing_board_t *playing_board, game_t *game);

/*
* Returns the next spot of the playing board from the start_index in a given direction.
*/
spot_t *get_next(playing_board_t *playing_board, int direction, int start_index);

/*
* Frees the members of playing_board and then the playing_board itself
*/
void free_playing_board(playing_board_t *playing_board);

/*
* Changes spot to default color.
*/
void reverse_spot(spot_t *spot, game_t *game);

/*
* Fills a spot with given color.
*/
void fill_spot(spot_t *spot, game_t *game, int color);

void build_game_walls(playing_board_t *playing_board, game_t *game);

void generate_point(playing_board_t *playing_board);

#endif
