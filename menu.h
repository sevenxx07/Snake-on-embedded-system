/**
 * Module for drawing menu and handling input from user
 * in interacting with menu 
 * Also setting the game 
 * @file menu.h
 * @author Michal Vaverka (vavermic@fel.cvut.cz) & Sára Veselá (veselsa1@fel.cvut.cz)
 * @date 2021-27-05
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef MENU_H
#define MENU_H

#include "utils.h"
#include "font_types.h"

#define BORDER_WIDTH 4
#define MAIN_MENU_GAP_SIZE 50
#define SETTINGS_GAP_SIZE 40
#define FONT_HEIGHT 16

#define MENU_SELECTION_Y_OFFSET 100
#define SETTINGS_SELECTION_Y_OFFSET 58
#define MENU_TEXT_Y_OFFSET 60
#define SETTINGS_TEXT_X_OFFSET 30

#define MENU_POINTER_AREA_HEIGHT 46
#define MENU_POINTER_AREA_WIDTH 472

#define SETTINGS_POINTER_AREA_HEIGHT 42
#define SETTINGS_POINTER_AREA_WIDTH 206

#define SECOND_CHOICE_X_OFFSET 146
#define CHOICE_WIDTH 72
#define TEXT_BOX_HEIGHT 48
#define TEXT_BOX_Y_OFFSET 172

#define ZERO_IN_ASCI 48

#define NEW_GAME -3
#define OPEN_SETTINGS -4
#define QUIT -1
/*
* Menu initialization, everything needed for the menu is created and set.
*/
game_t *menu_init(unsigned char* parlcd_mem_base, unsigned int* playing_area, unsigned char* led_mem_base);

/*
* Drawing first menu - start, settings, quit
*/
void draw_menu(unsigned char *parlcd_mem_base, unsigned int* area);

#endif