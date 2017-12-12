#include "fct_loop_setting.h"

void fct_loop_setting::init(sequencer* seq, char* name){
	_seq = seq;
	fct_clbk::set_fct_name(name);
}

void fct_loop_setting::on_push(uint8_t btn_id){
	track* t = _seq->get_current_track();
}
void fct_loop_setting::on_release(uint8_t btn_id){
}
void fct_loop_setting::update_ui(){
}
void fct_loop_setting::on_start(){
	track* t = _seq->get_current_track();
	
}
