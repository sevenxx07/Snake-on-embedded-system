#include "end_game.h"
#include "utils.h"
#include "font_types.h"
#include "lcd.h"
#include "led.h"
#include <time.h>
#include <unistd.h>

#define WIDTH 480
#define HEIGHT 320

void start_endgame(game_t *game, snake_t *first_snake, snake_t *second_snake)
{
    font_descriptor_t *font = &font_rom8x16;
    clear_area(game->parlcd_mem_base, game->playing_area);
    draw_text_center(game->playing_area, "GAME OVER", WIDTH/2, HEIGHT/3, 5, font, 0xffffff);
    if(first_snake->score > second_snake->score){
        draw_text_center(game->playing_area, "BLUE WON!", WIDTH/2, HEIGHT/2 + 30, 4, font, 0x0000ff);
        set_left_led_RGB(game->led_mem_base, 0x0000ff);
        set_right_led_RGB(game->led_mem_base, 0x0000ff);
    }else if(first_snake->score == second_snake->score){
        draw_text_center(game->playing_area, "DRAW!", WIDTH/2, HEIGHT/2 + 30, 4, font, 0x0000ff);
        set_left_led_RGB(game->led_mem_base, 0xff0000);
        set_right_led_RGB(game->led_mem_base, 0xff0000);
    }else{
        draw_text_center(game->playing_area, "GREEN WON!", WIDTH/2, HEIGHT/2 + 30, 4, font, 0x00ff00);
        set_left_led_RGB(game->led_mem_base, 0x00ff00);
        set_right_led_RGB(game->led_mem_base, 0x00ff00);
    }
    redraw_lcd(game->parlcd_mem_base, game->playing_area);
    int length = 4294967295;
    for(int i = 0; i < 3; i++){
        set_led_strip(game->led_mem_base, length);
        sleep(1);
        set_led_strip(game->led_mem_base, 0);
        sleep(1);
    }
}
