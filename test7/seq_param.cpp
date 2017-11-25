#include "seq_param.h"

seq_param::seq_param(){
}
seq_param::~seq_param(){
}

void seq_param::init(sequencer* const s){
	_s = s;
	param::_lm.set_led_x(LED_R_IDX, 0);	
}

void seq_param::on_push(uint8_t btn_id){
	param::_lm.toogle_led_x(LED_R_IDX, btn_id);;
	// TODO set param title
	fct_clbk* fc = _s->get_fct(btn_id);
	if(fc)
		Serial.println(fc->fct_clbk::_fct_name);
	else 
		Serial.println("unknown option");
}

void seq_param::on_release(uint8_t btn_id){
	_s->set_current_param(btn_id);
}

void seq_param::update_ui(){
}


