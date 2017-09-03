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

#ifndef _LIB_CLK_H_
#define _LIB_CLK_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include <elapsedMillis.h>

class clk {
	private:
		uint16_t _step_cnt;
		uint8_t	 _max_step;
		uint16_t _bpm;
		uint32_t _ms; 
		elapsedMillis _elapsed_ms;
		// diff from 0 and less than -1: divider
		// diff from 0 and sup than 1: multiplier
		int _operation; 
		boolean _resync;
		
	
	public:
		clk();
		~clk();
		
		uint32_t clk_get_ms();
		uint16_t clk_get_step_cnt();
		uint32_t clk_get_elapsed_ms();
		uint16_t clk_get_bpm();
		
		int clk_set_ms(uint32_t);
		int clk_set_bpm(uint16_t);	
		int clk_bpms_to_bpm(uint32_t);
		int clk_get_operator();

		void clk_set_max_step(uint8_t);
		
		boolean clk_set_operation(int, uint32_t);
		void clk_sync_intern(uint32_t);
		uint32_t clk_reset();

		uint32_t clk_elapsed();
		uint32_t clk_sync_divider(uint32_t, uint16_t);
		uint32_t clk_sync_multiplier(uint32_t);
		uint32_t clk_sync(uint32_t, uint16_t);
		
		uint32_t master_sync(uint32_t, uint16_t);
};

#endif
