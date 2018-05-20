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

#ifndef _LIB_GATE_H_
#define _LIB_GATE_H_

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif

#include <stdint.h>
#include <elapsedMillis.h>

#define GATE_HIGH		127
#define GATE_LOW		0

#define DEFAULT_VEL		127
#define GATE_OFF		0

#define GATE_ERROR		0
#define GATE_STARTED	1
#define GATE_FINISHED	2

class gate {
	private:
		uint8_t _gate_value;
		uint8_t _gate_off;
		uint32_t _gate_len;
		elapsedMillis _elapsed_ms;
		uint8_t _port;
		int	_gate_state;	

	public:
		gate();
//		gate(uint8_t, uint8_t (*hw_wr)(uint8_t, uint8_t));
		gate(uint8_t, uint8_t (*hw_wr)(uint8_t, uint8_t));
		~gate();
	
//		void set_gate_value(boolean);
		void set_gate_len(uint32_t);
		void set_hw_cbck(uint8_t, uint8_t (*hw_wr)(uint8_t, uint8_t));	
		void set_gate_trig_lvl(bool trig_lvl, uint8_t);

		int rst_gate(uint8_t);
		int rst_gate();
		int upd_gate();		
};
#endif
