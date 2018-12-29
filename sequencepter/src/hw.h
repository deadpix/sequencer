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

#ifndef __SRC_HW_H__
#define __SRC_HW_H__

#define LED_MATRIX_NR_LEDS		MATRIX_NR_ROW
#define LED_MATRIX_NR_GROUND		MATRIX_NR_COL
#define LED_MATRIX_NR_COLORS		3
#define NR_LEDS				(LED_MATRIX_NR_LEDS*LED_MATRIX_NR_GROUND)

#define LED_OFF_IDX			0x0
#define LED_R_IDX			0x1
#define LED_G_IDX			0x2
#define LED_B_IDX			0x4
#define LED_RG_IDX			(LED_R_IDX|LED_G_IDX)
#define LED_RB_IDX			(LED_R_IDX|LED_B_IDX)
#define LED_GB_IDX			(LED_G_IDX|LED_B_IDX)
#define LED_GBR_IDX			(LED_GB_IDX|LED_R_IDX)


#include <stdint.h>

class hw {
	public:
		virtual void refresh_matrix(uint16_t id) = 0;
		virtual void upd_pxl(uint16_t id, uint8_t color, uint8_t brightness) = 0;
};

#endif
