#include "midi_controller_param.h"

void midi_controller_param::redraw_midi_controller_param(){
	for(int i=0; i<MATRIX_NR_ROW*MATRIX_NR_COL;i++){
		param::_lm.clr_n_restore(i,BACKGROUND);
	}
	param::_lm.save_n_set(LED_R_IDX,last_conf_->midi_out, BACKGROUND);	
	// display keyboard starting from row 2
	last_conf_->kb.display_keys(&(param::_lm), 2);	
	last_conf_->kb.display_scale(&(param::_lm), 2);
	last_conf_->kb.display_root(&(param::_lm), 2);	
}

void midi_controller_param::init(midi_controller* mc){
	mc_ = mc;
	last_conf_ = mc_->get_midi_controller_conf();
	redraw_midi_controller_param();
}

void midi_controller_param::on_push(uint8_t btn_id){
	if(btn_id < 16){
		// change midi out
	}
	else if(btn_id >= 16 && btn_id < 32){
		// KBD event
	
	}
	else if(btn_id == 32){
		// octave down
	}
	else if(btn_id == 33){
		// octave up

	}
	else if(btn_id >= 40 && ((btn_id % MATRIX_NR_COL) < (MATRIX_NR_COL / 2))){
		// select scale
	}
}

void midi_controller_param::on_release(uint8_t btn_id){

}

void midi_controller_param::update_ui(uint32_t mst_ms, uint16_t mst_step){

}

void midi_controller_param::param_on_enter(){
	struct midi_controller_conf* mcc = mc_->get_midi_controller_conf();
	if(mcc != last_conf_){
		// redraw everything
		last_conf_ = mcc;
		redraw_midi_controller_param();
	}
}
void midi_controller_param::param_on_leave(){

}
