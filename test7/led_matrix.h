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

#ifndef __LED_MATRIX_H__
#define __LED_MATRIX_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h" 

#define LED_MATRIX_NR_LEDS			MATRIX_NR_ROW
#define LED_MATRIX_NR_GROUND		MATRIX_NR_COL
#define LED_MATRIX_NR_COLORS		3
#define NR_LEDS				(LED_MATRIX_NR_LEDS*LED_MATRIX_NR_GROUND)


typedef struct {
	mat_row_bmp_t bitmap[LED_MATRIX_NR_COLORS];
} led_t;

class led_matrix {
	private:
		led_t led_arr[LED_MATRIX_NR_GROUND];

	public:
		led_matrix();
		~led_matrix();

		led_t* get_led_arr(void);
		led_t get_led(uint8_t);

		int set_led_x(uint8_t, uint16_t);
		int clr_led_x(uint8_t, uint16_t);
		int toogle_led_x(uint8_t, uint16_t);

		int set_led_x_coor(uint8_t, uint16_t, uint16_t);
		int clr_led_x_coor(uint8_t, uint16_t, uint16_t);
		int toogle_led_x_coor(uint8_t, uint16_t, uint16_t);

		void dump_led_matrix(void);
};

#endif
