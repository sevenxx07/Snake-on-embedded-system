#include <stdlib.h>
#include <stdio.h>

#include "playing_board.h"
#include "game.h"
#include "queue.h"
#include "snake.h"

void innit_spot(spot_t *spot, int index) 
{
    spot->contains_point = false;
    spot->is_snake = false;
    spot->total_index = index;
}

playing_board_t *create_board(game_t *game, pthread_mutex_t *mtx_c, thread_data_t *thread_data) 
{
    playing_board_t *ret = malloc(sizeof(playing_board_t));

    ret->mtx = mtx_c;
    ret->thread_data = thread_data;

    spot_t *spots = malloc(sizeof(spot_t) * SPOT_COUNT);
    for (int i = 0; i < SPOT_COUNT; i++) {
        innit_spot(&spots[i], i);
    }

    ret->play_spots = spots;

    ret->first_snake = create_snake(true, 0x0000ff, false);
    ret->second_snake = create_snake(false, 0x00ff00, false);

    ret->game = game;

    return ret;
}

spot_t *get_next(playing_board_t *playing_board, int direction, int start_index) 
{
    int new_index = start_index + direction;

    if (playing_board->play_spots[new_index].is_snake || playing_board->play_spots[new_index].is_wall) {
        return NULL;
    }

    return &playing_board->play_spots[new_index];
}

void reverse_spot(spot_t *spot, game_t *game) 
{
    int offset_x = spot->total_index % BOARD_WIDTH;
    int offset_y = spot->total_index / BOARD_WIDTH;

    for (int i = 0; i < SPOT_DIMENSION; i++) {
        for (int j = 0; j < SPOT_DIMENSION; j++) {
            game->playing_area[(offset_x * SPOT_DIMENSION + i) + LCD_WIDTH * (offset_y * SPOT_DIMENSION + j)] = 0x000000;
        }
    }
}

void fill_spot(spot_t *spot, game_t *game, int color) 
{
    int offset_x = spot->total_index % BOARD_WIDTH;
    int offset_y = spot->total_index / BOARD_WIDTH;

    for (int i = 0; i < SPOT_DIMENSION; i++) {
        for (int j = 0; j < SPOT_DIMENSION; j++) {
            game->playing_area[(offset_x * SPOT_DIMENSION + i) + (offset_y * SPOT_DIMENSION + j) * LCD_WIDTH] = color;
        }
    }
}

void free_playing_board(playing_board_t *playing_board) 
{
    free(playing_board->play_spots);
    kill_snake(playing_board->first_snake, playing_board);
    kill_snake(playing_board->second_snake, playing_board);
    free(playing_board->first_snake);
    free(playing_board->second_snake);
    free(playing_board);
}

void build_game_walls(playing_board_t *playing_board, game_t *game) 
{
    for (int i = 0; i < BOARD_WIDTH; i++) {
        fill_spot(&playing_board->play_spots[i], game, 0xffffff);
        playing_board->play_spots[i].is_wall = true;
        fill_spot(&playing_board->play_spots[SPOT_COUNT - (i + 1)], game, 0xffffff);
        playing_board->play_spots[SPOT_COUNT - (i + 1)].is_wall = true;
    }

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        fill_spot(&playing_board->play_spots[i * BOARD_WIDTH], game, 0xffffff);
        playing_board->play_spots[i * BOARD_WIDTH].is_wall = true;
        fill_spot(&playing_board->play_spots[(i + 1) * BOARD_WIDTH - 1], game, 0xffffff);
        playing_board->play_spots[(i + 1) * BOARD_WIDTH - 1].is_wall = true;
    }
}

void generate_point(playing_board_t *playing_board) 
{
    int random_int;
    spot_t *curr_spot;
    while (true) {
        random_int = rand() % SPOT_COUNT;
        curr_spot = &playing_board->play_spots[random_int];
        if (!curr_spot->is_snake && !curr_spot->is_wall) {
            curr_spot->contains_point = true;
            fill_spot(curr_spot, playing_board->game, 0xffffff);
            break;
        }
    }
    playing_board->point = curr_spot;
}
