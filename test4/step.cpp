#include "step.h"

step::step(){
	flag_active = false;
	gate_len 	= 20;
}

step::~step(){
}

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
	return gate_len;
}

void step::set_step_gate_len(uint8_t len){
	gate_len = len;
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
