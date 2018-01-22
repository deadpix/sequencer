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

static led_state pool[NR_LEDS][3]; 

static led_state* get_led_state_from_pool(uint16_t nr){
	led_state* ret = NULL;
	for(int i=0;i<3;i++){
		if(!pool[nr][i]._in_use){
			ret = &(pool[nr][i]);
			ret->_in_use = true;
		}
	}
	return ret;
}

static void release_led_state(led_state* ls){
	ls->_in_use = false;
}


led_matrix::led_matrix(){
	for(int i=0;i<LED_MATRIX_NR_GROUND;i++){
		for(int j=0;j<LED_MATRIX_NR_COLORS;j++){
			led_arr[i].bitmap[j] = 0x0;
		}
	}
	for(int i=0;i<NR_LEDS;i++){
//		bmp[i] = 0x0;
//		bmp[i][0] = 0x0;
//		bmp[i][1] = 0x0;
	}
}

led_matrix::~led_matrix(){
}

led_t* led_matrix::get_led_arr(){
	return led_arr;
}

uint8_t led_matrix::get_led_x_state(uint8_t color, uint16_t x, uint16_t y){
	return (led_arr[x].bitmap[color] >> y) & 0x1;
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


void led_matrix::save_n_set(uint8_t color, uint16_t nr, uint8_t ground){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t idx;
	bool flg = false; 
	bool first = false;

	led_state* ls = new led_state(color, ground);
//	led_state* ls = get_led_state_from_pool(nr);
	int len = led_list[nr].size();

	if(len == 0){
		led_list[nr].add(ls);
		set_led_x(color, nr);
	} else {
		led_state* tmp;
		for(int i=0;i<len;i++){
			tmp = led_list[nr].get(i);
			if(tmp->_prio < ls->_prio){
				led_list[nr].add(i,ls);
				flg = true;
				break;
			}
		}
		if(!flg){
			led_list[nr].add(len, ls);
		}
		if(led_list[nr].get(0) == ls){
			led_ovw(color, nr);
		}	
	}

/*
	if(ground == BACKGROUND){
		Serial.println(bmp[nr][1]);
		if(bmp[nr][1]){
			bmp[nr][0] = bmp[nr][1];
			bmp[nr][1] = color;
			Serial.println("ouaich");
		} 
		else {
			// save the current led pix state (0xBGR)
			for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
				bmp[nr][0] |= get_led_x_state(i, x, y) << i;
			}
	
			for_eachset_bit(idx,(uint16_t *) &color, LED_MATRIX_NR_COLORS){
				set_led_x_coor(idx,x,y);
			}
		}
	}
	else {
		for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
			bmp[nr][1] |= get_led_x_state(i, x, y) << i;
		}
		for_eachset_bit(idx,(uint16_t *) &color, LED_MATRIX_NR_COLORS){
			set_led_x_coor(idx,x,y);
		}	
	}
*/
}

void led_matrix::clr_n_restore(uint16_t nr, uint8_t ground){
	uint16_t x = nr / LED_MATRIX_NR_LEDS;
	uint16_t y = nr % LED_MATRIX_NR_LEDS;
	uint16_t color;
	
	led_state* ls = led_list[nr].shift();
	clr_led_x(ls->_color, nr);
	delete ls;
//	release_led_state(ls);
	
	if( (ls = led_list[nr].get(0)) ){
		set_led_x(ls->_color, nr);
	}
/*	
	if(ground == BACKGROUND){
//		Serial.print("restore background ");
//		Serial.println(color);
		color = bmp[nr][0];
		bmp[nr][0] = 0x0;
		if(bmp[nr][1]){
			uint16_t tmp = bmp[nr][1];
			bmp[nr][1] = color;
			color = tmp;
		}
		// restore led pix state
		for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
			clr_led_x_coor(i, x, y);
			set_led_x_state(i, x, y, (color >> i) & 0x1);
		}
	}
	else {
		// restore led pix state
		for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
			clr_led_x_coor(i, x, y);
			set_led_x_state(i, x, y, (bmp[nr][1] >> i) & 0x1);
		}
		bmp[nr][1] = 0x0;
	}
*/
}
/*
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
*/
void led_matrix::led_off(uint16_t nr){
	for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
		clr_led_x(i, nr);
	}
}
void led_matrix::led_ovw(uint8_t color, uint16_t nr){
	for(int i=0;i<LED_MATRIX_NR_COLORS;i++){
		clr_led_x(i, nr);
//		if(BIT::is_bit_set(color, i)) set_led_x(i,nr);
	}
	set_led_x(color, nr);
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
