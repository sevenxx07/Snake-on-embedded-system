#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "utils.h"
#include "game.h"
#include "lcd.h"
#include "led.h"
#include "menu.h"
#include "font_types.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "input_handler.h"
#include "playing_board.h"
#include "snake.h"
#include "end_game.h"

#define BASE_TICK_SPEED 0.3f

pthread_mutex_t mtx_c;
pthread_cond_t character_read_c;
thread_data_t read_thread_data_c;

int main(int argc, char *argv[]) {
  //init playing area, lcd and led representation
	unsigned int* playing_area;
	playing_area = (unsigned int*)malloc(LCD_WIDTH * LCD_HEIGHT * (sizeof(unsigned int)));
	
  unsigned char* parlcd_mem_base;
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	if (parlcd_mem_base == NULL)
    exit(1);
	parlcd_hx8357_init(parlcd_mem_base);

  unsigned char *led_mem_base;
  led_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  if (led_mem_base == NULL)
   	exit(1);

  clear_area(parlcd_mem_base, playing_area);

  //introduction scene
  draw_introduction(playing_area, parlcd_mem_base, led_mem_base);
  set_left_led_RGB(led_mem_base, 0);
  set_right_led_RGB(led_mem_base, 0);

  //menu
  menu:
  clear_area(parlcd_mem_base, playing_area);
  game_t *game = menu_init(parlcd_mem_base, playing_area, led_mem_base);
  if (game == NULL) {
    clear_area(parlcd_mem_base, playing_area);
    printf("Game ended.\n"); 
  } else {
    //game
    playing_board_t *board = create_board(game, &mtx_c, &read_thread_data_c);

    pthread_mutex_init(&mtx_c, NULL); // initialize mutex with default attributes
    pthread_cond_init(&character_read_c, NULL);
    read_thread_data_c.quit = false;
    read_thread_data_c.last_read = ' ';
    pthread_t threads[1];

    clear_area(parlcd_mem_base, playing_area);

    pthread_create(&threads[0], NULL, input_thread, &read_thread_data_c);

    build_game_walls(board, game);
    redraw_lcd(parlcd_mem_base, playing_area);
    innit_snakes(board);
    redraw_lcd(parlcd_mem_base, playing_area);

    while (true) {
        if (update_game(game, board)) {
          break;
        }
        redraw_lcd(parlcd_mem_base, playing_area);
        fracSleep(BASE_TICK_SPEED / game->speed);
    }
    pthread_mutex_lock(&mtx_c);
    read_thread_data_c.quit = true;
    pthread_mutex_unlock(&mtx_c);
    pthread_mutex_destroy(&mtx_c);

    start_endgame(game, board->first_snake, board->second_snake);
    goto menu;
  }
  set_left_led_RGB(led_mem_base, 0);
  set_right_led_RGB(led_mem_base, 0);
  clear_area(parlcd_mem_base, playing_area);
  free(game);
  free(playing_area);

  return 0;
}
