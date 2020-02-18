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

#include "hw_lu.h"
#include <hw_debug.h>
#include "../perf.h"

#define TRACE_PERF 0

#if TRACE_PERF == 1
static perf p;
#endif

static const uint8_t id_lut[NR_LEDS] = { 
	 7,  6,  5,  4,  3,  2,  1,  0, 
	15, 14, 13, 12, 11, 10,  9,  8,
	23, 22, 21, 20, 19, 18, 17, 16,
	31, 30, 29, 28, 27, 26, 25, 24,
	39, 38, 37, 36, 35, 34, 33, 32,
	47, 46, 45, 44, 43, 42, 41, 40,
	55, 54, 53, 52, 51, 50, 49, 48,
	63, 62, 61, 60, 59, 58, 57, 56
};

hw_lu::hw_lu(CRGB* lu_matrix){
	_lu_matrix = lu_matrix;
	for(int i=0;i<3; i++){
		_sqcpt_to_lu_color[i][0] = CRGB::Black;
		_sqcpt_to_lu_color[i][1] = CRGB::Red;
		_sqcpt_to_lu_color[i][2] = CRGB::Blue;
		_sqcpt_to_lu_color[i][3] = CRGB::Magenta;
		_sqcpt_to_lu_color[i][4] = CRGB::Green;
		_sqcpt_to_lu_color[i][5] = CRGB::Yellow;
		_sqcpt_to_lu_color[i][6] = CRGB::Cyan;
		_sqcpt_to_lu_color[i][7] = CRGB::White;
	}
}

void hw_lu::refresh_matrix(uint16_t id){
	FastLED.show();
}

void hw_lu::upd_pxl(uint16_t id, uint8_t color, uint8_t brightness){
	_lu_matrix[id_lut[id]] = _sqcpt_to_lu_color[brightness][color];
}
