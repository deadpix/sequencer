#include "seq_param.h"

seq_param::seq_param(){
}
seq_param::~seq_param(){
}

void seq_param::init(sequencer* const s){
	_s = s;
	for(int i=0;i<(MATRIX_NR_COL*MATRIX_NR_ROW);i++){
		if(_s->get_fct(i))
			param::_lm.set_led_x(LED_R_IDX, i);
	}
	if(_s->get_fct(_s->current_param_id))
		param::_lm.set_led_x(LED_GBR_IDX, _s->current_param_id);
}

void seq_param::on_push(uint8_t btn_id){
	param::_lm.toogle_led_x(LED_R_IDX, btn_id);;
	// TODO set param title
	fct_clbk* fc = _s->get_fct(btn_id);
	if(fc)
		Serial.println(fc->fct_clbk::get_fct_name());
	else 
		Serial.println("unknown option");
}

void seq_param::on_release(uint8_t btn_id){
	_s->set_current_param(btn_id);
}

void seq_param::update_ui(){
}
void seq_param::param_on_enter(){
}
void seq_param::param_on_leave(){
	fct_clbk* fc = _s->get_fct(_s->get_current_param());
	if(fc)
		fc->on_start();
}


