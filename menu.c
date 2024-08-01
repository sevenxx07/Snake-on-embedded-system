#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>
#include <fcntl.h>

#include "game.h"
#include "lcd.h"
#include "font_types.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "menu.h"
#include "game.h"

char *int_to_string(int to_convert) 
{
    char *ret = malloc(sizeof(char) * 3);
    if (to_convert / 100 == 1) {
        ret[0] = '1';
        ret[1] = '0';
        ret[2] = '0';
    } else {
        ret[0] = '0';
        ret[1] = ZERO_IN_ASCI + to_convert / 10;
        ret[2] = ZERO_IN_ASCI + to_convert % 10;
    }
    return ret;
}

void draw_settings_pointer(unsigned char *parlcd_mem_base, unsigned int *playing_area, int curr_pointer) 
{
    invert_colors(playing_area, BORDER_WIDTH, SETTINGS_SELECTION_Y_OFFSET + (curr_pointer * (FONT_HEIGHT + SETTINGS_GAP_SIZE)), SETTINGS_POINTER_AREA_HEIGHT, SETTINGS_POINTER_AREA_WIDTH);
    redraw_lcd(parlcd_mem_base, playing_area);
}

void draw_menu_pointer(unsigned char *parlcd_mem_base, unsigned int *playing_area, int curr_pointer) 
{
    invert_colors(playing_area, BORDER_WIDTH, MENU_SELECTION_Y_OFFSET + (curr_pointer * (FONT_HEIGHT + MAIN_MENU_GAP_SIZE)), MENU_POINTER_AREA_HEIGHT, MENU_POINTER_AREA_WIDTH);
    redraw_lcd(parlcd_mem_base, playing_area);
}

void redraw_speed(game_t *game) 
{
    font_descriptor_t *font = &font_rom8x16;
    char *speed_string = int_to_string(game->speed);
    clear_chosen_area(game->playing_area, LCD_WIDTH/2, TEXT_BOX_Y_OFFSET, CHOICE_WIDTH, TEXT_BOX_HEIGHT);
    draw_text(game->playing_area, speed_string, LCD_WIDTH/2, TEXT_BOX_Y_OFFSET, 3, font, 0xffffff);
    free(speed_string);
}

void redraw_choices(game_t *game, int add) 
{
    invert_colors(game->playing_area, LCD_WIDTH/2, MENU_TEXT_Y_OFFSET+ add * (FONT_HEIGHT + SETTINGS_GAP_SIZE), SETTINGS_POINTER_AREA_HEIGHT, CHOICE_WIDTH);
    invert_colors(game->playing_area, LCD_WIDTH/2 + SECOND_CHOICE_X_OFFSET, MENU_TEXT_Y_OFFSET+ add * (FONT_HEIGHT + SETTINGS_GAP_SIZE), SETTINGS_POINTER_AREA_HEIGHT, CHOICE_WIDTH);
}

void draw_choices(game_t *game) 
{
    font_descriptor_t *font = &font_rom8x16;

    int x = LCD_WIDTH/2;
    int y = MENU_TEXT_Y_OFFSET;
    if (game->type == false) {
        invert_colors(game->playing_area, x, y, SETTINGS_POINTER_AREA_HEIGHT, CHOICE_WIDTH);
    } else {
        invert_colors(game->playing_area, x + SECOND_CHOICE_X_OFFSET, y, SETTINGS_POINTER_AREA_HEIGHT, CHOICE_WIDTH);
    }

    y += FONT_HEIGHT + SETTINGS_GAP_SIZE;

    if (game->difficulty == false) {
        invert_colors(game->playing_area, x, y, SETTINGS_POINTER_AREA_HEIGHT, CHOICE_WIDTH);
    } else {
        invert_colors(game->playing_area, x + SECOND_CHOICE_X_OFFSET, y, SETTINGS_POINTER_AREA_HEIGHT, CHOICE_WIDTH);
    } 

    y += FONT_HEIGHT + SETTINGS_GAP_SIZE;

    char *speed_string = int_to_string(game->speed);

    draw_text(game->playing_area, speed_string, x, y, 3, font, 0xffffff);

    free(speed_string);
}

void draw_settings(unsigned int *playing_area) 
{
    font_descriptor_t *font = &font_rom8x16;

    int y = SETTINGS_SELECTION_Y_OFFSET + 2;
    draw_text(playing_area, "TYPE:", SETTINGS_TEXT_X_OFFSET, y, 3, font, 0xffffff);
    draw_text(playing_area, "PVC / CVC", LCD_WIDTH/2, y, 3, font, 0xffffff);

    y += (font->height + SETTINGS_GAP_SIZE);
    draw_text(playing_area, "DIFF:", SETTINGS_TEXT_X_OFFSET, y, 3, font, 0xffffff);
    draw_text(playing_area, "ESY / HRD", LCD_WIDTH/2, y, 3, font, 0xffffff);

    y += (font->height + SETTINGS_GAP_SIZE);
    draw_text(playing_area, "SPEED:", SETTINGS_TEXT_X_OFFSET, y, 3, font, 0xffffff);

    y += (font->height + SETTINGS_GAP_SIZE);
    draw_text(playing_area, "CONFIRM", SETTINGS_TEXT_X_OFFSET, y, 3, font, 0xffffff);
}


int perform_action_settings (game_t *game, int curr_pointer) 
{
    switch (curr_pointer) {
        case 0:
            game->type = game->type == false ? true : false;
            redraw_choices(game, 0);
            break;
        case 1:
            game->difficulty = game->difficulty == false ? true : false;
            redraw_choices(game, 1);
            break;
        case 2:
            game->speed++;
            if (game->speed > 3) {
                game->speed = 1;
            }
            redraw_speed(game);
            break;
        case 3:
            return QUIT;
    }

    return curr_pointer;
}

int perform_action_menu(int curr_pointer) 
{
    //default wont ever happen but must be included because of compiler :)
    switch(curr_pointer) {
        case 0:
            return NEW_GAME;
        case 1:
            return OPEN_SETTINGS;
        case 2:
            return QUIT;
        default:
            return NEW_GAME;
    }
}

int update_pointer(int curr_pointer, _Bool settings, int max_value, game_t *game) 
{
    char input;

    while (1) {
        read(STDIN_FILENO, &input, 1);
        if (input == ' ') {
            continue;
        } else if (input == 'w') {
            curr_pointer = curr_pointer == 0 ? 0 : (curr_pointer - 1);
            return curr_pointer;
        } else if (input == 's') {
            curr_pointer = curr_pointer == max_value ? max_value : (curr_pointer + 1);
            return curr_pointer;
        } else if (input == 'e') {
            if (settings) {
                return perform_action_settings(game, curr_pointer);
            } else {
                return perform_action_menu(curr_pointer);
            }
      }
      input = ' ';
    }

    return curr_pointer;
}


void open_settings(game_t *game) 
{
    int settings_pointer = 0;

    clear_area(game->parlcd_mem_base, game->playing_area);
    draw_settings(game->playing_area);
    draw_choices(game);
    draw_settings_pointer(game->parlcd_mem_base, game->playing_area, settings_pointer);

    while (1) {
        invert_colors(game->playing_area, 4, SETTINGS_SELECTION_Y_OFFSET + (settings_pointer * (FONT_HEIGHT + SETTINGS_GAP_SIZE)), SETTINGS_POINTER_AREA_HEIGHT, SETTINGS_POINTER_AREA_WIDTH);

        settings_pointer = update_pointer(settings_pointer, 1, 3, game);
        if (settings_pointer == QUIT) {
            break;
        }

        draw_settings_pointer(game->parlcd_mem_base, game->playing_area, settings_pointer);
    }

    clear_area(game->parlcd_mem_base, game->playing_area);
} 

void draw_menu(unsigned char *parlcd_mem_base, unsigned int* area)
{
    font_descriptor_t *font = &font_rom8x16;
    set_borders(parlcd_mem_base, area, BORDER_WIDTH);
    int y = MENU_TEXT_Y_OFFSET;
    draw_text_center(area, "MENU", LCD_WIDTH/2, y, 5, font, 0xff6655);

    y += (font->height + MAIN_MENU_GAP_SIZE);
    draw_text_center(area, "START", LCD_WIDTH/2, y, 4, font, 0xffffff);

    y += (font->height + MAIN_MENU_GAP_SIZE);
    draw_text_center(area, "SETTINGS", LCD_WIDTH/2, y, 4, font, 0xffffff);

    y += (font->height + MAIN_MENU_GAP_SIZE);
    draw_text_center(area, "QUIT", LCD_WIDTH/2, y, 4, font, 0xffffff);
}

game_t* menu_init(unsigned char* parlcd_mem_base, unsigned int* playing_area, unsigned char* led_mem_base) 
{
    int menu_pointer = 0;
    game_t *new_game = create_game(parlcd_mem_base, led_mem_base, playing_area);
    game_t *ret = NULL;

    struct termios tio, tioOld;

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    tcgetattr(STDIN_FILENO, &tio);
    tioOld = tio; // backup
    cfmakeraw(&tio);
    tio.c_oflag |= OPOST;
    tcsetattr(STDIN_FILENO, TCSANOW, &tio);

    clear_area(parlcd_mem_base, playing_area);
    draw_menu(parlcd_mem_base, playing_area);
    draw_menu_pointer(parlcd_mem_base, playing_area, menu_pointer);

    while (1) {
        invert_colors(playing_area, BORDER_WIDTH, MENU_SELECTION_Y_OFFSET + (menu_pointer * (FONT_HEIGHT + MAIN_MENU_GAP_SIZE)), MENU_POINTER_AREA_HEIGHT, MENU_POINTER_AREA_WIDTH);
        menu_pointer = update_pointer(menu_pointer, 0, 2, ret);
        if (menu_pointer == QUIT) {
            break;
        } else if (menu_pointer == NEW_GAME) {
            ret = new_game;
            break;
        } else if (menu_pointer == OPEN_SETTINGS) {
            open_settings(new_game);
            draw_menu(parlcd_mem_base, playing_area);
            menu_pointer = 0;
        }

        draw_menu_pointer(parlcd_mem_base, playing_area, menu_pointer);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);

    return ret;
}
