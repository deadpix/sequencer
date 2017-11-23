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
#ifndef __BTN_STATE_H__
#define __BTN_STATE_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
//#include <LinkedList.h>

#define BTN_MATRIX_ROW	MATRIX_NR_ROW
#define BTN_MATRIX_COL	MATRIX_NR_COL

typedef struct {
	mat_row_bmp_t bitmap;
} btn_t;

typedef struct {
	uint16_t btn_id;
	unsigned long timestamp;
} active_btn_t;

class btn_state {
	private:
		btn_t btn_arr[BTN_MATRIX_COL];
//		LinkedList<active_btn_t*> list_active_btns;
	
	public:
		btn_state();
		~btn_state();		

		int set_btn_active_coor(uint16_t x, uint16_t y);
		int clr_btn_active_coor(uint16_t x, uint16_t y);

		int set_btn_active(uint16_t id);
		int clr_btn_active(uint16_t id);
};

#endif
