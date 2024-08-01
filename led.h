/**
 * Module for manipulation with RGB right and left led 
 * and led strip
 * @file led.h
 * @author Sára Veselá (veselsa1@fel.cvut.cz)
 * @date 2021-25-05
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef LED_H
#define LED_H
/*
* Setting left RGB led according to given color
*/
void set_left_led_RGB(unsigned char* led_mem_base, unsigned int color);

/*
* Setting right RGB led according to given color
*/
void set_right_led_RGB(unsigned char* led_mem_base, unsigned int color);

/*
* Setting let strip according to given number
*/
void set_led_strip(unsigned char* led_mem_base, unsigned int length);

#endif