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

#ifndef __LED_MATRIX_NT_H__
#define __LED_MATRIX_NT_H__

#include <stdint.h>
#include "types.h" 
#include "led_state.h"
#include <LinkedList.h>
#include "hw.h"

#define BACKGROUND			0
#define FOREGROUND1			1
#define FOREGROUND2			2
#define NR_LEVELS			3

struct led_status {
	uint8_t color[NR_LEVELS];
	uint8_t bmp;
};

class led_matrix {
	private:
		struct led_status _led_status_arr[NR_LEDS];
		hw* _hw;

	public:
		led_matrix();
		~led_matrix();

		struct led_status* get_led_status(uint16_t led_id);
		void set_hw(hw* h);
		hw*  get_hw();

		int  save_n_set(uint8_t, uint16_t, uint8_t);
		int  save_n_toogle(uint8_t, uint16_t, uint8_t);
		int  save_n_ovw(uint8_t, uint16_t, uint8_t);
		int  save_n_set_dfl(uint8_t, uint16_t, uint8_t);
		int  clr_n_restore(uint16_t, uint8_t);
};

#endif
