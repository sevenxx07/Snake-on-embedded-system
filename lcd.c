#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "lcd.h"
#include "font_types.h"
#include "utils.h"

void fill_pixel(int x, int y, unsigned int color, unsigned int *area, int pixel_size)
{
	for(int i = 0; i < pixel_size; i++){
		for(int j = 0; j < pixel_size; j++){
    		if (x >= 0 && x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT) {
    			area[(x+i) + LCD_WIDTH * (y+j)] = color;
  			}
		}
	} 
}

void invert_colors(unsigned int *playing_area, int start_x, int start_y, int area_height, int area_width) 
{
    for (int i = 0; i < area_height; i++) {
        for (int j = 0; j < area_width; j++) {
            if (playing_area[LCD_WIDTH * (start_y + i) + (start_x + j)] == 0xffffff) {
                playing_area[LCD_WIDTH * (start_y + i) + (start_x + j)] = 0x000000;
            } else {
                playing_area[LCD_WIDTH * (start_y + i) + (start_x + j)] = 0xffffff;
            }
        }
    }
}

void redraw_lcd(unsigned char *parlcd_mem_base, unsigned int *area)
{
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for (int ptr = 0; ptr < LCD_WIDTH * LCD_HEIGHT ; ptr++) {
		parlcd_write_data(parlcd_mem_base, area[ptr]);
	}
}

void draw_letter(unsigned int *area, char letter, int x, int y, int pixel_size, font_descriptor_t *font, unsigned int color)
{
   int offset = ((int)letter) *16 ;
   for (int i = 0; i < font->height; i++){
      uint16_t line = font_rom8x16.bits[offset+i];
      for (int j = 0; j < font->maxwidth; j++){
         uint16_t mask = 1 << (15 - j);
         if (line & mask)
         {
            int rectx = x + j * pixel_size;
            int rexty = y + i * pixel_size;
			fill_pixel(rectx, rexty, color, area, pixel_size);
         }
      }
   }
}

void draw_text(unsigned int *area, char *s, int x, int y, int pixel_size, font_descriptor_t *font, unsigned int color)
{
   for (int i = 0; i < strlen(s); ++i){
      draw_letter(area, s[i], x, y, pixel_size, font, color);
      x = x + 1 + font->maxwidth * pixel_size;
   }
}

//writes a text based on its center (input coords are where we want to have it=)
void draw_text_center(unsigned int *area, char *text, int x, int y, int pixel_size, font_descriptor_t *font, unsigned int color)
{
   int text_width = 0;
   int text_height = font->height * pixel_size;
   for (int i = 0; i < strlen(text); ++i){
      text_width += font->maxwidth * pixel_size;
   }
   x = x - text_width / 2;
   y = y - text_height / 2;
   for (int i = 0;  i < strlen(text); ++i){
      draw_letter(area, text[i], x, y, pixel_size, font, color);
      x = x + 1 + font->maxwidth * pixel_size;
   }
}

void clear_chosen_area(unsigned int *playing_area, int start_x, int start_y, int area_width, int area_height) 
{
   for (int i = 0; i < area_height; i++) {
      for (int j = 0; j < area_width; j++) {
         playing_area[(start_y + i) * LCD_WIDTH + (start_x + j)] = 0x000000;
      }
   }
}
