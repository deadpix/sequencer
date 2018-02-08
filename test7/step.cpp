#include "step.h"

step::step(){
	flag_active = false;
	_linked = false;
	gate_len_per = 20;
	flg_gate = false;
	_note.velocity = 127;
	_note.pitch = 37;
}

step::~step(){
}

void step::step_set_note(uint8_t vel, uint16_t pitch){
	_note.velocity = vel;
	_note.pitch = pitch;
}

void step::reset_gate(){
	gate_elapsed = 0;
	flg_gate = true;
}
bool step::upd_gate(){
	bool res = false;

	if( flg_gate && !_linked && (gate_elapsed > gate_len_ms) ){
		res = true;
		flg_gate = false;
	}
	return res;
}

//int step::check_gate(uint32_t ms){
//}

boolean step::is_step_active(){
	return flag_active;
}

void step::set_step_active(){
	flag_active = true;
}

void step::clr_step_active(){
	flag_active = false;
}

uint8_t step::get_step_gate_len(){
	return gate_len_per;
}

void step::set_step_gate_len(uint32_t ms, uint8_t len){
	gate_len_per = len;
	gate_len_ms = ms;
}

void step::upd_step_gate_len(uint32_t ms_ref){
	gate_len_ms = ms_ref * gate_len_per / 100;
}

uint8_t step::get_step_id(){
	return step_id;
}

void step::set_step_id(uint8_t id){
	step_id = id;
}

void step::toogle_step(){
	flag_active = !flag_active;
}

boolean step::step_status(){
	return flag_active;
}

void step::link_step(){
	_linked = true;
//	gate_len_per = 100;
//	gate_len_ms = 0;
}
void step::unlink_step(){
	_linked = false;
}
bool step::is_step_linked(){
	return _linked;
}
