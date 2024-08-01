#include <stdio.h>
#include <stdlib.h>

#include "mzapo_parlcd.h"
#include "mzapo_regs.h"
#include "led.h"

void set_left_led_RGB(unsigned char* led_mem_base, unsigned int color)
{
  *(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_RGB1_o) = color;
}

void set_right_led_RGB(unsigned char* led_mem_base, unsigned int color)
{
	*(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_RGB2_o) = color;
}

void set_led_strip(unsigned char* led_mem_base, unsigned int length)
{
	*(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_LINE_o) = length;
	//works but the lenght is converted to binary
}
