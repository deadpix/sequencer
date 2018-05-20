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

#include <stdint.h>
//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif

#include "types.h" 
#include "led_state.h"
#include <LinkedList.h>

#define LED_MATRIX_NR_LEDS			MATRIX_NR_ROW
#define LED_MATRIX_NR_GROUND		MATRIX_NR_COL
#define LED_MATRIX_NR_COLORS		3
#define NR_LEDS				(LED_MATRIX_NR_LEDS*LED_MATRIX_NR_GROUND)

#define LED_R_IDX			0x1
#define LED_G_IDX			0x2
#define LED_B_IDX			0x4
#define LED_RG_IDX			(LED_R_IDX|LED_G_IDX)
#define LED_RB_IDX			(LED_R_IDX|LED_B_IDX)
#define LED_GB_IDX			(LED_G_IDX|LED_B_IDX)
#define LED_GBR_IDX			(LED_GB_IDX|LED_R_IDX)

#define BACKGROUND			0
#define FOREGROUND1			1
#define FOREGROUND2			2
#define NR_LEVELS			3


typedef struct {
	mat_row_bmp_t bitmap[LED_MATRIX_NR_COLORS];
} led_t;

struct led_status {
	uint8_t color[NR_LEVELS];
	uint8_t bmp;
};

class led_matrix {
	private:
		led_t led_arr[LED_MATRIX_NR_GROUND];
		struct led_status _led_status_arr[NR_LEDS];

	public:
		led_matrix();
		~led_matrix();

		led_t* get_led_arr(void);
		led_t get_led(uint8_t);

		uint8_t get_led_x_state(uint8_t, uint16_t, uint16_t);
		void set_led_x_state(uint8_t, uint16_t, uint16_t, uint8_t);

		int set_led_x(uint8_t, uint16_t);
		int clr_led_x(uint8_t, uint16_t);
		int toogle_led_x(uint8_t, uint16_t);

		int set_led_x_coor(uint8_t, uint16_t, uint16_t);
		int clr_led_x_coor(uint8_t, uint16_t, uint16_t);
		int toogle_led_x_coor(uint8_t, uint16_t, uint16_t);

//		void save_n_set(uint8_t, uint16_t);
//		void clr_n_restore(uint16_t);

		int  save_n_set(uint8_t, uint16_t, uint8_t);
		int  save_n_toogle(uint8_t, uint16_t, uint8_t);
		void clr_n_restore(uint16_t, uint8_t);

		void led_off(uint16_t);
		void led_ovw(uint8_t, uint16_t);

		void dump_led_matrix(void);
};

#endif
