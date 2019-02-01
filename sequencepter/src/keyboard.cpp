#include "keyboard.h"


keyboard::keyboard(){
	root_key_ = mC3;
	cur_scale_ = &scales[1];
	playable_scale_ = &scales[0];
}

void keyboard::display_keys(){
	// display playable keys
	for(int i=0;i<playable_scale_->nr_notes;i++){
//		uint8_t offset = keys_offset[]
//		save_n_set(LED_R_IDX, (i+));
	}
}
