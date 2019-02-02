#ifndef __SRC_KEYBOARD_H__
#define __SRC_KEYBOARD_H__

#include "led_matrix.h"
#include "midi_cst.h"

#define KEYBOARD_NR_INTERVALS	12 // 12 semi-tones
#define KEYBOARD_NR_COL		8

struct scale {
	uint8_t nr_notes;
	uint8_t intervals[KEYBOARD_NR_INTERVALS];
};


// btn id of the key
const uint8_t keys_offset[] = {0,9,1,10,2,3,12,4,13,5,14,6};
const int8_t keys_to_midi_offset[] = {0,2,4,5,7,9,11,-1,-1,1,3,-1,6,8,10,-1};

const struct scale scales[] = {
	{ 12, {0,1,2,3,4,5,6,7,8,9,10,11} }, // semi-tones
	{ 7,  {0,2,4,5,7,9,10} },
};

enum NOTE {
	DO=0,
	DOd=1, REb=1,
	RE=2,
	REd=3, MIb=3,
	MI=4,
	FA=5,
	FAd=6, SOLb=6,
	SOL=7,
	SOLd=8, LAb=8,
	LA=9,
	LAd=10, SIb=10,
	SI=11,
};
class keyboard {
	private:
		uint8_t root_key_;
		uint8_t octave_; 
		const struct scale* cur_scale_;
		const struct scale* playable_scale_;
//		led_matrix* lm_;

	public:
		keyboard();
		~keyboard(){};

		uint8_t get_root_key(){return root_key_;};
		void set_root_key(uint8_t root_key){ root_key_ = root_key;};
		
		const scale* get_cur_scale(){return cur_scale_;};
		void set_cur_scale(const struct scale* s){cur_scale_ = s;};

		const scale* get_playable_scale(){return playable_scale_;};
		void set_playable_scale(const struct scale* s){playable_scale_ = s;};

//		void set_led_matrix(led_matrix* lm){lm_ = lm;};

		void display_keys(led_matrix* lm, uint8_t row);
		void display_scale(led_matrix* lm, uint8_t row);
		void display_root(led_matrix* lm, uint8_t row);

		int get_note_offset_C_centered(uint8_t key, uint16_t* offset);	
};

#endif
