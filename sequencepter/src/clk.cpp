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

#include "clk.h"
#include "types.h"
#include <hw_debug.h>
#include "../interrupts.h"

#define MAX_DIVIDER 	16
#define DELAY_MS		50
#define MAX_STEP		(MATRIX_NR_ROW*MATRIX_NR_COL)

static uint32_t bpm_to_ms(uint16_t bpm){
	return (60000/bpm);
}

static uint16_t ms_to_bpm(uint32_t ms){
	return (uint16_t)(ms/60000);
}

static uint16_t bpms_to_bpm(uint32_t bpms){
	return (uint16_t) (60000/bpms);
}

clk::clk(){
	_step_cnt	= 0;
	_max_step	= MAX_STEP;
	_bpm 		= 90;
	_elapsed_ms = 0;
	_ms 		= bpm_to_ms(_bpm);
	_ms_ref 	= _ms;
	_numerator	= 1;
	_denominator	= 1;
}

clk::~clk(){
}

uint32_t clk::clk_get_ms(){
	return _ms;
}
uint32_t clk::clk_get_ms_lock(){
	uint32_t ms;
	DISABLE_IRQ();
	ms = _ms;
	ENABLE_IRQ();
	return ms;
}
uint16_t clk::clk_get_step_cnt(){
	return _step_cnt;
}
uint32_t clk::clk_get_elapsed_ms(){
	return _elapsed_ms;
}
uint16_t clk::clk_get_bpm(){
	return _bpm;
}

void clk::clk_set_max_step(uint8_t max_step){
	_max_step = max_step;
}

int clk::clk_bpms_to_bpm(uint32_t bpms){
	uint16_t bpm = bpms_to_bpm(bpms);
	clk_set_bpm(bpm);
	return 0;
}
int clk::clk_set_bpm(uint16_t new_bpm){
	int ret = 0;
	if(new_bpm > _bpm)
		ret = 1;
	_bpm = new_bpm;
	_ms = bpm_to_ms(_bpm);
	_ms_ref = _ms;

	return ret;
}

uint32_t clk::clk_set_ratio(uint32_t ms_ref, uint8_t numerator, uint8_t denominator){
//	bool ret = false;
	if(_numerator != numerator || _denominator != denominator){
		_numerator = numerator;
		_denominator = denominator;
		
		if(ms_ref == 0){
			_ms = _ms_ref * _numerator / _denominator;
		}
		else {
			_ms = ms_ref * _numerator / _denominator;
			_ms_ref = ms_ref;
		}

		_bpm = ms_to_bpm(_ms);		
		_step_cnt = 0;
//		ret = true;
	}

	// need to set lower limit for _ms (2ms, 5ms, 10ms ???)

	return _ms;
}

uint32_t clk::clk_sync_ratio(uint32_t ms, uint16_t step){
	UNUSED(step);
	uint32_t ret = 0;

//	Serial.print("ms ");	
//	Serial.println(ms);	

	_ms_ref = ms;
	_ms = ms * _numerator / _denominator;
	_bpm = ms_to_bpm(_ms);

	ret = _ms;
	_elapsed_ms = 0;
	_step_cnt = 0;

	return ret;
}

uint32_t clk::clk_elapsed(){
	uint32_t ret = 0;
	if(_elapsed_ms >= _ms){
	
//		Serial.print("_elapsed_ms");	
//		Serial.println(_elapsed_ms);	
		_elapsed_ms = 0;
		_step_cnt = (_step_cnt + 1) % _max_step;
		ret = _ms;
	}
	return ret;
}

uint32_t clk::master_sync_ratio(uint32_t mst_ms, uint16_t* mst_cnt){
	uint32_t ret = 0;

//	Serial.print("numerator ");
//	Serial.println(_numerator);

	if(mst_ms > 0){
	       if(*mst_cnt >= _numerator){
//		       Serial.println("sync");
			ret = clk_sync_ratio(mst_ms, *mst_cnt);
			*mst_cnt = 1;
	       } else {
		       *mst_cnt = *mst_cnt + 1;
	       }
		
	} else if(_step_cnt < (_denominator - 1)){
//	} else if(_step_cnt < (_denominator)){
//		Serial.print("_step_cnt ");
//		Serial.println(_step_cnt);
		ret = clk_elapsed();
//		if(ret){
//			Serial.print("ret ");
//			Serial.println(ret);
//
//		}
			
	} 
	return ret;
}
