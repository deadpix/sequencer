#include "fct_step_count.h"

void fct_step_count::init(sequencer* seq, char* name){
	_seq = seq;
	fct_clbk::set_fct_name(name);
}

void fct_step_count::on_push(uint8_t btn_id){
	track* t = _seq->get_current_track();
}
void fct_step_count::on_release(uint8_t btn_id){
}
void fct_step_count::update_ui(){
}
void fct_step_count::on_start(){
	track* t = _seq->get_current_track();
	
}
