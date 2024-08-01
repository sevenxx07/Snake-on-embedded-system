/**
 * Module for manipulation with LCD display
 * @file ldc.h
 * @author Sára Veselá (veselsa1@fel.cvut.cz)
 * @date 2021-25-05
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef LCD_H
#define LCD_H
#include "font_types.h"
#include "utils.h"

/*
* Filling pixel with color
*/
void fill_pixel(int x, int y, unsigned int color, unsigned int *area, int pixel_size);

/*
* Invert colors in given area
*/
void invert_colors(unsigned int *playing_area, int start_x, int start_y, int area_height, int area_width);


/*
* Redraw whole LCD display after updating playing area
*/
void redraw_lcd(unsigned char *parlcd_mem_base, unsigned int *area); 

/*
* Draw one letter according to fonts
*/
void draw_letter(unsigned int *area, char letter, int x, int y, int pixel_size, font_descriptor_t *font, unsigned int color);

/*
* Draw given text on LCD display
*/
void draw_text(unsigned int *area, char *s, int x, int y, int pixel_size, font_descriptor_t *font, unsigned int color);

/*
* Draw given text on centre of LCD display
*/
void draw_text_center(unsigned int *area, char *text, int x, int y, int pixel_size, font_descriptor_t *font, unsigned int color);

/*
* Clears an area of given parameters (sets the pixels to black)
*/
void clear_chosen_area(unsigned int *playing_area, int start_x, int start_y, int area_width, int area_height);

#endif
