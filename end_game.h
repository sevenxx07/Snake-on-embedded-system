/**
 * Module for generating and handling score_points.
 *
 * @file end_game.h
 * @author Michal Vaverka (vavermic@fel.cvut.cz)
 * @date 2021-31-05
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef END_GAME_H
#define END_GAME_H

#include "utils.h"
#include "game.h"

/*
* Starts the endgame screen, which quits / starts a new game once the game has ended.
*/
void start_endgame(game_t *game, snake_t *first_snake, snake_t *second_snake);

#endif
