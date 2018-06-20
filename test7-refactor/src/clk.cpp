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
	_operation	= 1;
	_resync 	= false;
	_numerator	= 1;
	_denominator	= 1;
}

clk::~clk(){
}

void clk::clk_sync_intern(uint32_t ms){
	if(_operation < 0){
		_ms = ms * abs(_operation);
		_bpm = ms_to_bpm(_ms);
	} else {
		_ms = ms / _operation;
		_bpm = ms_to_bpm(_ms);
	}
}

uint32_t clk::clk_get_ms(){
	return _ms;
}
uint16_t clk::clk_get_step_cnt(){
	return _step_cnt;
}
uint32_t clk::clk_get_elapsed_ms(){
	return _elapsed_ms;
}
int clk::clk_get_operator(){
	return _operation;
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
}

int clk::clk_set_ms(uint32_t new_ms){
	int ret = 0;
	if(new_ms < _ms)
		ret = 1;
	_ms = new_ms;
	_bpm = ms_to_bpm(_ms);
	
	return ret;
}

int clk::clk_set_bpm(uint16_t new_bpm){
	int ret = 0;
	if(new_bpm > _bpm)
		ret = 1;
	_bpm = new_bpm;
	_ms = bpm_to_ms(_bpm);

	return ret;
}

bool clk::clk_set_ratio(uint32_t ms_ref, uint8_t numerator, uint8_t denominator){
	bool ret = false;
	if(_numerator != numerator || _denominator != denominator){
		_numerator = numerator;
		_denominator = denominator;
	
		_ms = ms_ref * _numerator / _denominator;
		_bpm = ms_to_bpm(_ms);
		
		_step_cnt = 0;
		ret = true;
	}

	// need to set lower limit for _ms (2ms, 5ms, 10ms ???)

	return ret;
}

bool clk::clk_set_operation(int op, uint32_t ms_ref){
	bool ret = true;

	if(op < 0){
		_ms = ms_ref * abs(op);
		_bpm = ms_to_bpm(_ms);
		_operation = op;
	} 
	else if(op > 0){
		_ms = ms_ref / op;
		_bpm = ms_to_bpm(_ms);
		_operation = op;
	}
	else if(op == 0){
		_operation = 1;
		ret = false;
	}
	if( (abs(op) > _max_step) || (_ms <= 0) ){
		ret = false;
	}
	return ret;
}

uint32_t clk::clk_sync_divider(uint32_t ms, uint16_t step){
	uint8_t divider = abs(_operation);
	uint32_t ret = 0;
	_ms = ms * divider;
	_bpm = ms_to_bpm(_ms);

	if( ((step +1) % divider) == 0 ){
		ret = _ms;
		_elapsed_ms = 0;
	}
	
	return ret;
}

uint32_t clk::clk_sync_multiplier(uint32_t ms){
	//TODO neeed to check step number for sync
	_ms = ms / _operation;
	_bpm = ms_to_bpm(_ms);
	_elapsed_ms = 0;
	_step_cnt = 0;
	return _ms;
}

uint32_t clk::clk_reset(){
	_elapsed_ms = 0;
	_step_cnt = 0;
	return _ms;
}

uint32_t clk::clk_sync(uint32_t ms, uint16_t step){
	uint32_t ret;
	if(_operation < 0){
		ret = clk_sync_divider(ms, step);
	}
	else {
		ret = clk_sync_multiplier(ms);
	}
	return ret;
}

uint32_t clk::clk_sync_ratio(uint32_t ms, uint16_t step){
	uint32_t ret = 0;

	_ms = ms * _numerator / _denominator;
	_bpm = ms_to_bpm(_ms);

//	dbg::printf("_ms=%d _bmp=%d\n",_ms,_bpm);		

//	Serial.print("step ");
//	Serial.print(step);
//	Serial.print(" _numerator ");
//	Serial.print(_numerator);
//	Serial.print(" _ms ");
//	Serial.println(_ms);
//	Serial.print();
//	Serial.print();

//	if( ((step+1) % _numerator) == 0 ){
//	if( ((step+1) % _denominator) == 0 ){
		ret = _ms;
		_elapsed_ms = 0;
		_step_cnt = 0;
//	}
	return ret;
}

uint32_t clk::clk_elapsed(){
	uint32_t ret = 0;
	if(_elapsed_ms > _ms){
		_elapsed_ms = 0;
		_step_cnt = (_step_cnt + 1) % _max_step;
		ret = _ms;
	}
	return ret;
}

uint32_t clk::master_sync(uint32_t mst_ms, uint16_t mst_cnt){
	uint32_t ret = 0;

	/* master has been synced or received new tick */
	if(mst_ms > 0){
		ret = clk_sync(mst_ms, mst_cnt);
	}
	/* "multiplied" slave clk sync itself until _step_cnt = _operation     */
	/* when _step_cnt = _operation, slave clk has to be synced with master */
	else if( (_operation > 0) && (_step_cnt < (_operation - 1)) ){
		ret = clk_elapsed(); 
	}
	return ret;
}
uint32_t clk::master_sync_ratio(uint32_t mst_ms, uint16_t* mst_cnt){
	uint32_t ret = 0;

//	if(mst_ms){
//		Serial.print("mst_ms ");
//		Serial.print(mst_ms);
//		Serial.print(" _numerator ");
//		Serial.print(_numerator);
//		Serial.print(" mst_cnt ");
//		Serial.println(*mst_cnt);
//	}

	if(mst_ms > 0){
	       if(*mst_cnt >= _numerator){
			ret = clk_sync_ratio(mst_ms, *mst_cnt);
			*mst_cnt = 1;
	       } else {
//			Serial.println("inc mst counter");	
	       	       *mst_cnt = *mst_cnt + 1;

	       }
		
	} else if(_step_cnt < (_denominator - 1)){
		ret = clk_elapsed();
	}


//	if(mst_ms > 0 && _step_cnt >= _numerator


//	/* master has been synced or received new tick */
//	if(mst_ms > 0 && _step_cnt >= (_denominator - 1)){
//		ret = clk_sync_ratio(mst_ms, mst_cnt);
//	}
//	/* with ratio, clock are synced on the numerator and slave clock self- */
//	/* check if counter is less than denom-1                               */
//	else if( /*(_denominator > _numerator) &&*/ (_step_cnt < (_denominator - 1)) ){
//		ret = clk_elapsed();
//		if(ret){
//			Serial.print("_denominator ");
//			Serial.print(_denominator);
//			Serial.print(" _dstep_cnt ");
//			Serial.print(_step_cnt);
//			Serial.print(" ret ");
//			Serial.println(ret);
//		}
//
//	}
	return ret;
}
