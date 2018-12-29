#include "step.h"
#include "led_matrix.h"
#include <hw_debug.h>

step::step(){
	flag_active = false;
	_linked = false;
	gate_len_per = 20;
	_flg_gate = false;
	_note.velocity = 127;
	_note.pitch = 37;
	_gate_bmp = 0x0;
//	_color = LED_OFF_IDX;
	_color = LED_R_IDX;
	_clk_def.numerator = 1;
	_clk_def.denominator = 1;
}

step::~step(){
	dbg::printf("delete step\n");
}

void step::step_set_note(uint8_t vel, uint16_t pitch){
	_note.velocity = vel;
	_note.pitch = pitch;
}

bool step::reset_gate(){
	gate_elapsed = 0;
	_flg_gate = true;
	return (_gate_bmp & GATE_UP);
}
bool step::upd_gate(){
	bool res = false;

	if( (_gate_bmp & GATE_DW) 
		&& (gate_elapsed > gate_len_ms) 
		&& _flg_gate){
		res = true;
		_flg_gate = false;
	}
	return res;
}

//int step::check_gate(uint32_t ms){
//}

bool step::is_step_active(){
	return flag_active;
}

void step::set_step_active(){
	flag_active = true;
//	_color = LED_R_IDX;
	_gate_bmp = GATE_ON;
}

void step::clr_step_active(){
	flag_active = false;
//	_color = LED_OFF_IDX; 
	_gate_bmp = GATE_OFF;
}

void step::set_step_up(){
	flag_active = true;
	_gate_bmp |= GATE_UP;
}
void step::set_step_dw(){
	flag_active = true;
	_gate_bmp |= GATE_DW;
}
void step::set_step_off(){
	flag_active = true;
	_gate_bmp = GATE_OFF;
}

void step::set_next_step(step* s){
	_next = s;
}
step* step::get_next_step(){
	return _next;
}

void step::set_step_color(uint8_t color){
	_color = color;
}
uint8_t step::get_step_color(){
	return _color;
}

void step::set_clk(clk* c){
	_c = c;
}

clk* step::get_clk(){
	return _c;
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

//void step::toogle_step(){
//	flag_active = !flag_active;
//}

bool step::step_status(){
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
