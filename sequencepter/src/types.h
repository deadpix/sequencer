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

#ifndef __TYPES_H__
#define __TYPES_H__

#define UNUSED(x) (void)(x)

#define MATRIX_NR_ROW			8
#define MATRIX_NR_COL			8
#define LED_MATRIX_NR_COLORS		3
#define NR_LEDS				(LED_MATRIX_NR_LEDS*LED_MATRIX_NR_GROUND)
#define NR_STEP				NR_LEDS
#define DEFAULT_STEP_PER_SEQ		8

#define LED_COLOR_RED_INDEX		0
#define LED_COLOR_GREEN_INDEX		1
#define LED_COLOR_BLUE_INDEX		2



#if MATRIX_NR_ROW <= 8
typedef volatile uint8_t  mat_row_bmp_t;
#elif MATRIX_NR_ROW <= 16
typedef volatile uint16_t mat_row_bmp_t;
#elif MATRIX_NR_ROW <= 32
typedef volatile uint32_t mat_row_bmp_t;
#endif

// Pulse Per Quarter Note (PPQN)
#define MIDI_SYNC_PPQN	24
// Pulse Per Note (PPN)
#define MIDI_SYNC_PPN	(MIDI_SYNC_PPQN*1)



#define MICRO_OLED_GUI 	1

typedef uint8_t (*cbck_fct_rd)(uint8_t pin);


#define DEBUG	1

#define likely(x)               __builtin_expect(!!(x), 1)
#define unlikely(x)             __builtin_expect(!!(x), 0)
#define infinite_loop() do {} while (1)

#include <hw_debug.h>

#define assert_failed(p)                                            		\
        do {                                                            	\
		dbg::print("Assertion failed: ");				\
		dbg::print(__FILE__);						\
		dbg::print(" at line ");					\
		dbg::print(__LINE__);						\
                infinite_loop();                                          	\
        } while (0)                                                     	\

#if defined(DEBUG) 

#define ASSERT(p)                                                   		\
        do {                                                            	\
                if ( unlikely(!(p)) ) assert_failed(#p);                    	\
        } while (0)                                                     	\

#else

#define ASSERT(p) 

#endif



#endif
