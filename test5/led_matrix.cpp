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

#include "led_matrix.h"

led_matrix::led_matrix(){
	for(int i=0;i<LED_MATRIX_NR_LEDS;i++){
		for(int j=0;j<LED_MATRIX_NR_COLORS;j++){
			led_arr[i].bitmap[j] = 0x0;
		}
	}
}

led_matrix::~led_matrix(){
}

led_t* led_matrix::get_led_arr(){
	return led_arr;
}

int led_matrix::set_led_x_coor(uint8_t color, uint16_t x, uint16_t y){
	int res = 0;
	led_arr[x].bitmap[color] |= (1<<y);
	return res;
}

int led_matrix::clr_led_x_coor(uint8_t color, uint16_t x, uint16_t y){
	int res = 0;
	led_arr[x].bitmap[color] &= ~(1<<y);
	return res;
}

int led_matrix::set_led_x(uint8_t color, uint16_t nr){
	int x = nr / LED_MATRIX_NR_COLORS;
	int y = nr % LED_MATRIX_NR_GROUND;

	return set_led_x_coor(color,x,y);
}

int led_matrix::clr_led_x(uint8_t color, uint16_t nr){
	int x = nr / LED_MATRIX_NR_COLORS;
	int y = nr % LED_MATRIX_NR_GROUND;

	return clr_led_x_coor(color,x,y);
}

led_t led_matrix::get_led(uint8_t x){
	led_t tmp = led_arr[x];
	return tmp;
//	return led_arr[x];
}
