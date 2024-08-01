#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "lcd.h"
#include "led.h"
#include "game.h"
#include "playing_board.h"
#include "snake.h"

#define WIDTH 480
#define HEIGHT 320

void fracSleep(float sec) {
    struct timespec ts;
    ts.tv_sec = (int) sec;
    ts.tv_nsec = (sec - ((int) sec)) * 1000000000;
    nanosleep(&ts,NULL);
}

void draw_introduction(unsigned int* playing_area, unsigned char* parlcd_mem_base, unsigned char* led_mem_base)
{
  font_descriptor_t *font = &font_rom8x16;
  draw_text_center(playing_area, "SNAKE", WIDTH/2, HEIGHT/2, 7, font, 0xffffff);
  redraw_lcd(parlcd_mem_base, playing_area);
  set_left_led_RGB(led_mem_base, 0xffffff);
  set_right_led_RGB(led_mem_base, 0xffffff);
  int length = 2147483648;
  for(int i = 0; i < 32; i++){
    length = length>>1;
    set_led_strip(led_mem_base, ~length);
    fracSleep(0.1);
  }
}

game_t* create_game(unsigned char* parlcd_mem_base, unsigned char* led_mem_base, unsigned int* playing_area) 
{
  game_t* new_game = malloc(sizeof(game_t));
  if (new_game == NULL) {
    fprintf(stderr, "An error has ocurred while creating a new game\n");
    exit(-1);
  }
  new_game->parlcd_mem_base = parlcd_mem_base;
  new_game->led_mem_base = led_mem_base;
  new_game->playing_area = playing_area;
  new_game->difficulty = false;
  new_game->type = false;
  new_game->speed = 1;
  new_game->is_running = false;

  return new_game;
}

void set_borders(unsigned char *parlcd_mem_base, unsigned int* area, int pixel_size)
{
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      if(i == 0 || (i == HEIGHT - pixel_size && j < WIDTH - pixel_size) || (j == WIDTH - pixel_size && i < HEIGHT - pixel_size) || j == 0){
        fill_pixel(j, i, 0x25ffff, area, pixel_size);
      }
    }
  }
}

void clear_area(unsigned char *parlcd_mem_base, unsigned int* area){
	for (int ptr = 0; ptr < WIDTH * HEIGHT ; ptr++){
    area[ptr] = 0;
  }
	redraw_lcd(parlcd_mem_base, area);
}

int update_game(game_t *game, playing_board_t *playing_board)
{
  int ret = 0;

  move(playing_board->first_snake, playing_board);
  move(playing_board->second_snake, playing_board);

  if (!playing_board->first_snake->is_alive) {
    playing_board->first_snake->score = 0;
    ret = 1;
  }

  if(!playing_board->second_snake->is_alive){
    playing_board->second_snake->score = 0;
    ret = 1;
  }

  return ret;
}
