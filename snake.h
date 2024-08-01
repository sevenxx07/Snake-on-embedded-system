/**
 * Module of functions for snake interactions.
 *
 * @file snake.h
 * @author Michal Vaverka (vavermic@fel.cvut.cz)
 * @date 2021-02-06
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef SNAKE_H
#define SNAKE_H

#include "utils.h"

#define ILLEGAL_DIRECTION 0
#define SNAKE_1_START_POINT 78
#define SNAKE_2_START_POINT 88
#define DEFAULT_BODY_SIZE 3
#define MAX_DIR_SWITCH_TRIES 6

#define ONE_ROW_STEP 24
#define NUMBER_OF_DIRECTIONS 4

/*
* Creates a snake.
*/
snake_t *create_snake(bool is_player, int color, bool snake_nr);

/*
* Function to be called when snake should perform a move.
* Calls the right function to perform the move.
*/
void move(snake_t *snake, playing_board_t *playing_board);

/*
* Function to handle player movement.
*/
void move_player(playing_board_t *playing_board, snake_t *snake);

/*
* Function to handle ai movement.
*/
void move_ai_easy(playing_board_t *playing_board, snake_t *snake);

void move_ai_hard(playing_board_t *playing_board, snake_t *snake);

/*
* Frees allocated memory within snake and clears him from the playing_board.
*/
void kill_snake(snake_t *snake, playing_board_t *playing_board);

/*
* Adds the base spots to the queue in snake and draws them on the playing board.
*/
void innit_snakes(playing_board_t *playing_board);

#endif