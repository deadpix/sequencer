/*
 *	Copyright (c) 2017 Vincent "deadpix" Dupre
 *
 *	Author: Vincent Dupre (vdupre@gmail.com)
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "hw_sr.h"
#include "../bit.h"

hw_sr::hw_sr(){
	for(int i=0;i<LED_MATRIX_NR_GROUND;i++){
		for(int j=0;j<LED_MATRIX_NR_COLORS;j++){
			led_arr[i].bitmap[j] = 0x0;
		}
	}
}

hw_sr::~hw_sr(){
};

led_t* hw_sr::get_led_arr(){
	return led_arr;
}
led_t hw_sr::get_led(uint8_t x){
	return led_arr[x];
}
//int hw_sr::clr_led_x_coor(uint8_t color, uint16_t x, uint16_t y){
int hw_sr::set_led_x_coor(uint8_t color, uint16_t x, uint16_t y){
	int res = 0;
	led_arr[x].bitmap[color] |= (1<<y);
	return res;
}

int hw_sr::clr_led_x_coor(uint8_t color, uint16_t x, uint16_t y){
//int hw_sr::set_led_x_coor(uint8_t color, uint16_t x, uint16_t y){
	int res = 0;
	led_arr[x].bitmap[color] &= ~(1<<y);
	return res;
}

int hw_sr::toogle_led_x_coor(uint8_t color, uint16_t x, uint16_t y){
	int res = 0;
	led_arr[x].bitmap[color] ^= (1<<y);
	return res;
}
int hw_sr::set_led_x(uint8_t color, uint16_t nr){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t idx;

	for_eachset_bit(idx,(uint8_t *) &color, LED_MATRIX_NR_COLORS){
		set_led_x_coor(idx,x,y);
	}

	return 1;
}

int hw_sr::clr_led_x(uint8_t color, uint16_t nr){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t idx;

	for_eachset_bit(idx,(uint8_t *) &color, LED_MATRIX_NR_COLORS){
		clr_led_x_coor(idx,x,y);
	}
	return 1;
}

void hw_sr::led_off(uint16_t nr){
	for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
		clr_led_x((1<<i), nr);
	}
}

int hw_sr::toogle_led_x(uint8_t color, uint16_t nr){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t idx;

	for_eachset_bit(idx,(uint8_t *) &color, LED_MATRIX_NR_COLORS){
		toogle_led_x_coor(idx,x,y);
	}
	return 1;
}

void hw_sr::refresh_matrix(uint16_t id){
	
}
void hw_sr::upd_pxl(uint16_t id, uint8_t color, uint8_t brightness){
	led_off(id);
	set_led_x(color, id);
}
