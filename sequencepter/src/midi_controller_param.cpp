#include "midi_controller_param.h"

#define PARAM_KEYBOARD_ROW	2
static uint16_t play_bmp;

void midi_controller_param::redraw_midi_controller_param(){
	for(int i=0; i<MATRIX_NR_ROW*MATRIX_NR_COL;i++){
		param::_lm.clr_n_restore(i,BACKGROUND);
	}
	param::_lm.save_n_set(LED_R_IDX,last_conf_->midi_out, BACKGROUND);	
	// display keyboard starting from row 2
	last_conf_->kb.display_keys(&(param::_lm), PARAM_KEYBOARD_ROW, 0);	
	last_conf_->kb.display_scale(&(param::_lm), PARAM_KEYBOARD_ROW);
	last_conf_->kb.display_root(&(param::_lm), PARAM_KEYBOARD_ROW);	
}

void midi_controller_param::init(midi_controller* mc){
	mc_ = mc;
	last_conf_ = mc_->get_midi_controller_conf();
	redraw_midi_controller_param();
}

void midi_controller_param::on_push(uint8_t btn_id){
	if(btn_id < 16){
		// change midi out
		// clear previous out_id
		param::_lm.clr_n_restore(last_conf_->midi_out, BACKGROUND);
		last_conf_->midi_out = btn_id;
		param::_lm.save_n_set(LED_R_IDX, last_conf_->midi_out, BACKGROUND);
	}
	else if(btn_id >= 16 && btn_id < 32){
		// KBD event -> set root note
		int key = keys_to_midi_offset[btn_id%16];
		if(key > -1){
			last_conf_->kb.display_scale(&(param::_lm), PARAM_KEYBOARD_ROW);
//			last_conf_->kb.display_root(&(param::_lm), PARAM_KEYBOARD_ROW, key);
			last_conf_->kb.upd_root(&(param::_lm), PARAM_KEYBOARD_ROW, key);
		}	
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
