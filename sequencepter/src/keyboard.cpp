#include "keyboard.h"
#include <hw_debug.h>

keyboard::keyboard(){
	root_key_ = DO;
	octave_ = 4;
	cur_scale_ = &scales[1];
	playable_scale_ = &scales[0];
}

void keyboard::display_keys(led_matrix* lm, uint8_t row, uint8_t col){
	// need to be sure that row < 6
	if(cur_scale_->type == NOTE_TYPE){ 
		int row_offset;
		if(row > KEYBOARD_NR_COL - 2)
			return;

		row_offset = row * KEYBOARD_NR_COL;
		// display playable keys
		for(int i=0;i<playable_scale_->nr_notes;i++){
			lm->save_n_ovw(LED_R_IDX, row_offset + keys_offset[i], BACKGROUND);
		}
	}
	else {
		// drums are layered on 4row x 4col layer
		

	}
}
void keyboard::display_scale(led_matrix* lm, uint8_t row){
	if(cur_scale_->type == NOTE_TYPE){ 

		int row_offset;
		if(row > KEYBOARD_NR_COL - 2)
			return;
	
		row_offset = row * KEYBOARD_NR_COL;
		// display scale keys
		for(int i=0;i<cur_scale_->nr_notes;i++){
			uint8_t scale_idx = (root_key_ + cur_scale_->intervals[i]) % KEYBOARD_NR_INTERVALS;
			lm->save_n_ovw(LED_B_IDX, row_offset + keys_offset[scale_idx] ,BACKGROUND);
		}
	}
	else {


	}
} 
void keyboard::display_root(led_matrix* lm, uint8_t row){
	if(cur_scale_->type == NOTE_TYPE){ 
		int row_offset;
		if(row > KEYBOARD_NR_COL - 2)
			return;

		row_offset = row * KEYBOARD_NR_COL;
		lm->save_n_ovw(LED_G_IDX, row_offset + keys_offset[root_key_], BACKGROUND);
	} 
	else {


	}
}

void keyboard::upd_root(led_matrix* lm, uint8_t row,  uint8_t root_key){
	if(cur_scale_->type == NOTE_TYPE){ 
		int row_offset;
		if(row > KEYBOARD_NR_COL - 2)
			return;

		row_offset = row * KEYBOARD_NR_COL;

		lm->clr_n_restore( row_offset + keys_offset[root_key_],BACKGROUND);
		root_key_ = root_key;
		lm->save_n_ovw(LED_G_IDX, row_offset + keys_offset[root_key_], BACKGROUND);
	} else {


	}
}

int keyboard::get_midi_note(uint8_t key, uint16_t* midi_note){	
	int ret = 1;
	
	if(cur_scale_->type == NOTE_TYPE){

		int note = key%16;
		int octave_offset = octave_ + (key / 16) - 2;

		if( (keys_to_midi_offset[note] == -1) || (octave_offset < 0))
			ret = 0; 

		*midi_note = keys_to_midi_offset[note] + (KEYBOARD_NR_INTERVALS * octave_offset) + MIDI_C1_OFFSET;

	} 
	else if(cur_scale_->type == DRUM_TYPE) {
		uint8_t tmp1 = (key % 8) / 4 + (key / 32) * 2;
		uint8_t tmp2 = (key / 8) % 4;
		uint8_t tmp3 = (key % 4);
		
		*midi_note = MPC1000_PAD[tmp1][tmp2][tmp3];

	}
	return ret;
}
