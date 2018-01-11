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
#include "bit.h"

led_matrix::led_matrix(){
	for(int i=0;i<LED_MATRIX_NR_GROUND;i++){
		for(int j=0;j<LED_MATRIX_NR_COLORS;j++){
			led_arr[i].bitmap[j] = 0x0;
		}
	}
	for(int i=0;i<NR_LEDS;i++){
		bmp[i] = 0x0;
	}
}

led_matrix::~led_matrix(){
}

led_t* led_matrix::get_led_arr(){
	return led_arr;
}

uint8_t led_matrix::get_led_x_state(uint8_t color, uint16_t x, uint16_t y){
	return (led_arr[x].bitmap[color] << y) & 0x1;
}
void led_matrix::set_led_x_state(uint8_t color, uint16_t x, uint16_t y, uint8_t state){
	led_arr[x].bitmap[color] |= (state<<y);
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

int led_matrix::toogle_led_x_coor(uint8_t color, uint16_t x, uint16_t y){
	int res = 0;
	led_arr[x].bitmap[color] ^= (1<<y);
	return res;
}

int led_matrix::set_led_x(uint8_t color, uint16_t nr){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t idx;

	for_eachset_bit(idx,(uint16_t *) &color, LED_MATRIX_NR_COLORS){
		set_led_x_coor(idx,x,y);
	}

//	return set_led_x_coor(color,x,y);
	return 1;
}

int led_matrix::clr_led_x(uint8_t color, uint16_t nr){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t idx;

	for_eachset_bit(idx,(uint16_t *) &color, LED_MATRIX_NR_COLORS){
		clr_led_x_coor(idx,x,y);
	}
//	return clr_led_x_coor(color,x,y);
	return 1;
}

int led_matrix::toogle_led_x(uint8_t color, uint16_t nr){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t idx;

	for_eachset_bit(idx,(uint16_t *) &color, LED_MATRIX_NR_COLORS){
		toogle_led_x_coor(idx,x,y);
	}
	return 1;
//	return toogle_led_x_coor(color,x,y);
}

led_t led_matrix::get_led(uint8_t x){
	return led_arr[x];
}

void led_matrix::save_n_set(uint8_t color, uint16_t nr){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t idx;

	// save the current led pix state (0xBGR)
	for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
		bmp[nr] |= get_led_x_state(i, x, y) << i;
	}
	for_eachset_bit(idx,(uint16_t *) &color, LED_MATRIX_NR_COLORS){
		set_led_x_coor(idx,x,y);
	}
}
void led_matrix::clr_n_restore(uint16_t nr){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;

	// restore led pix state
	for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
		clr_led_x_coor(i, x, y);
		set_led_x_state(i, x, y, (bmp[nr] >> i) & 0x1);
	}
	bmp[nr] = 0x0;
}

void led_matrix::dump_led_matrix(){
	for(int i=0;i<LED_MATRIX_NR_GROUND;i++){
		for(int j=0;j<LED_MATRIX_NR_COLORS;j++){
			Serial.print("led_arr[");
			Serial.print(i);
			Serial.print("].bitmap[");
			Serial.print(j);
			Serial.print("] = ");
			Serial.println(led_arr[i].bitmap[j]);
		}
	}	
}
