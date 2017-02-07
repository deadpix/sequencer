#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif


#include <inttypes.h>

#include "gate.h"

static uint8_t (*_hw_wr_cbck)(uint8_t, bool);

gate::gate(){
	_gate_value = GATE_LOW;
	_gate_len = 0;
	_gate_state = GATE_FINISHED;
}

gate::gate(uint8_t port, uint8_t (*hw_wr)(uint8_t, bool)){
	_gate_value = GATE_LOW;
	_gate_len = 0;
	_gate_state = GATE_FINISHED;
	_port = port;
	_hw_wr_cbck = hw_wr;
}

gate::~gate(){
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

int gate::upd_gate(){
	if(!_hw_wr_cbck)	
		return GATE_ERROR;

	if(_gate_state != GATE_STARTED)
		return GATE_FINISHED;

	if(_elapsed_ms > _gate_len){
		_gate_value = !_gate_value;
		_hw_wr_cbck(_port, _gate_value);
		_gate_state = GATE_FINISHED;
	}
	return (int)_gate_state;
}
