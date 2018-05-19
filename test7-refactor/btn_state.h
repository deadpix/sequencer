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

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif

#include <stdint.h>

#include "types.h"
#include "bit.h"

#define BIT_PER_BYTE	8	
class btn_state {
	private:
		uint8_t short_push[MATRIX_NR_COL];
		uint8_t long_push[MATRIX_NR_COL];
	
	public:
		btn_state();
		~btn_state(){};		
		
		void set_short_push(uint8_t);
		void clr_short_push(uint8_t);
		void set_long_push(uint8_t);
		void clr_long_push(uint8_t);

		uint8_t get_nr_short_push();
		uint8_t	get_nr_long_push();

		uint8_t get_short_push_id(uint8_t*);
		uint8_t get_long_push_id(uint8_t*);
};

#endif
