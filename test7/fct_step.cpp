#include "fct_step.h"

void fct_step::init(sequencer* seq){
	_seq = seq;
	fct_clbk::_fct_name = "step ed";
}

void fct_step::on_push(uint8_t btn_id){
	track* t = _seq->get_current_track();
	if(t->arr_step[btn_id].is_step_active())
		t->arr_step[btn_id].clr_step_active();
	else 
		t->arr_step[btn_id].set_step_active();
	t->get_led_matrix()->toogle_led_x(LED_R_IDX, btn_id);
}
void fct_step::on_release(uint8_t btn_id){
}
void fct_step::update_ui(){
}