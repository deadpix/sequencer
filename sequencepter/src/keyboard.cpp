#include "keyboard.h"


keyboard::keyboard(){
	root_key_ = DO;
	octave_ = 4;
	cur_scale_ = &scales[1];
	playable_scale_ = &scales[0];
}

void keyboard::display_keys(led_matrix* lm, uint8_t row){
	// need to be sure that row < 6
	int row_offset;
	if(row > KEYBOARD_NR_COL - 2)
		return;

	row_offset = row * KEYBOARD_NR_COL;
	// display playable keys
	for(int i=0;i<playable_scale_->nr_notes;i++){
		lm->save_n_ovw(LED_R_IDX, row_offset + keys_offset[i], BACKGROUND);
	}
}
void keyboard::display_scale(led_matrix* lm, uint8_t row){
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
void keyboard::display_root(led_matrix* lm, uint8_t row){
	int row_offset;
	if(row > KEYBOARD_NR_COL - 2)
		return;

	lm->save_n_ovw(LED_G_IDX, row_offset + keys_offset[root_key_], BACKGROUND);
}

int keyboard::get_note_offset_C_centered(uint8_t key, uint16_t* midi_note){	
	int note = keys_offset[key%16];
	int octave_offset = octave_ + (key / 16) - 2;

	if( (keys_to_midi_offset[note] == -1) || (octave_offset < 0))
		return 0; 

	*midi_note = keys_to_midi_offset[note] + (KEYBOARD_NR_INTERVALS * octave_offset);
	return 1;
}
