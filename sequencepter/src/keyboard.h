#ifndef __SRC_KEYBOARD_H__
#define __SRC_KEYBOARD_H__

#include "led_matrix.h"
#include "midi_cst.h"

#define KEYBOARD_NR_INTERVALS	12 // 12 semi-tones

struct scale {
	uint8_t nr_notes;
	uint8_t intervals[KEYBOARD_NR_INTERVALS];
};

// btn id of the key
const uint8_t keys_offset[] = {0,9,2,10,3,4,12,5,13,6,14,7};

const struct scale scales[] = {
	{ 12, {0,1,2,3,4,5,6,7,8,9,10,11} }, // semi-tones
	{ 7, {0, 2, 4, 5, 7, 9, 10} },
};

class keyboard {
	private:
		uint16_t root_key_;
		const struct scale* cur_scale_;
		const struct scale* playable_scale_;
		led_matrix* lm_;

	public:
		keyboard();
		~keyboard(){};

		uint16_t get_root_key(){return root_key_;};
		void set_root_key(uint16_t root_key){ root_key_ = root_key;};
		
		const scale* get_cur_scale(){return cur_scale_;};
		void set_cur_scale(const struct scale* s){cur_scale_ = s;};

		const scale* get_playable_scale(){return playable_scale_;};
		void set_playable_scale(const struct scale* s){playable_scale_ = s;};

		void set_led_matrix(led_matrix* lm){lm_ = lm;};

		void display_keys();
};

#endif
