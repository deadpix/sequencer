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

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif


#include <inttypes.h>

#include "gate.h"

static uint8_t (*_hw_wr_cbck)(uint8_t, bool);

gate::gate(){
	_gate_value = GATE_HIGH;
	_gate_len = 0;
	_gate_state = GATE_FINISHED;
}

gate::gate(uint8_t port, uint8_t (*hw_wr)(uint8_t, bool)){
	_gate_value = GATE_HIGH;
	_gate_len = 0;
	_gate_state = GATE_FINISHED;
	_port = port;
	_hw_wr_cbck = hw_wr;
}

gate::~gate(){
}

void gate::set_gate_trig_lvl(bool trig_level){
	_gate_value = trig_level;
}

void gate::set_gate_len(uint32_t ms){
	_gate_len = ms;
}

void gate::set_hw_cbck(uint8_t port, uint8_t (*hw_wr)(uint8_t, bool)){
	_port = port;
	_hw_wr_cbck = hw_wr;
}

int gate::rst_gate(bool gate_val){
	if(!_hw_wr_cbck)	
		return GATE_ERROR;

	_gate_value = gate_val;
	_elapsed_ms = 0;
	_hw_wr_cbck(_port, _gate_value);
	_gate_state = GATE_STARTED;
	
	return GATE_STARTED;
}
int gate::rst_gate(){
	if(!_hw_wr_cbck)	
		return GATE_ERROR;

	_elapsed_ms = 0;
	_hw_wr_cbck(_port, _gate_value);
	_gate_state = GATE_STARTED;
	
	return GATE_STARTED;
}
int gate::upd_gate(){
	if(!_hw_wr_cbck)	
		return GATE_ERROR;

	if(_gate_state != GATE_STARTED)
		return GATE_FINISHED;

	if(_elapsed_ms > _gate_len){
		_hw_wr_cbck(_port, !_gate_value);
		_gate_state = GATE_FINISHED;
	}
	return (int)_gate_state;
}
