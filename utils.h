/**
 * Module of basic data types.
 *
 * @file utils.h
 * @author Michal Vaverka (vavermic@fel.cvut.cz) & Sára Veselá (veselsa1@fel.cvut.cz)
 * @date 2021-01-06
 * 
 * @copyright Copyright (c) 2021
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <pthread.h>

#define LCD_WIDTH 480
#define LCD_HEIGHT 320

typedef struct
{
    bool quit;
    char last_read;
    char last_input;
} thread_data_t;

typedef struct{
  unsigned char* parlcd_mem_base;
  unsigned char *led_mem_base;
  unsigned int* playing_area;
  bool difficulty; //easy - false
  bool type;       //pvc - false
  int speed;
  bool is_running;
} game_t;

typedef struct {
    bool is_snake;
    bool contains_point;
    bool is_wall;
    int total_index;
} spot_t;

typedef struct {
    spot_t *spot;
    void *next;
    void *prev;
} member_t;

typedef struct {
    member_t *head;
    member_t *tail;
} queue_t;

typedef struct {
    queue_t *snake_body;
    bool is_alive;
    bool is_player;
    int color;
    int direction;
    bool has_eaten;
    int score;
    bool snake_nr;
} snake_t;

typedef struct {
    spot_t *play_spots;
    snake_t *first_snake;
    snake_t *second_snake;
    game_t *game;
    pthread_mutex_t *mtx;
    thread_data_t *thread_data;
    unsigned int score_strip;
    spot_t *point;
} playing_board_t;

#endif
