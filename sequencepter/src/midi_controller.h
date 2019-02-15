#ifndef __TEST_PROJ_ONE_H__
#define __TEST_PROJ_ONE_H__

#include <LinkedList.h>

//#include "types.h"
#include "prog.h"
#include "led_matrix.h"
#include "led_toogle.h"
#include "keyboard.h"

struct midi_controller_conf {
	uint8_t id;
	uint8_t kb_mask[KEYBOARD_NR_ROW];
	keyboard kb;
	uint8_t midi_out;
};

class midi_controller : public prog {
	private:
		led_matrix lm_;
		LinkedList<led_toogle *> btn_animation_list_;
		struct midi_controller_conf conf_[MATRIX_NR_ROW];
		struct midi_controller_conf* current_conf_;
//		uint8_t kb_mask_[KEYBOARD_NR_ROW];
//		keyboard kb_;
//		uint8_t midi_out_;
	
	public:
		midi_controller();
		~midi_controller();

		led_matrix* get_led_matrix(void);
		void display_midi_keyboard(struct midi_controller_conf* c);
		void init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t));
//		uint8_t get_midi_out(){ return midi_out_;};
//		void set_midi_out(uint8_t out){ midi_out_ = out;};
		struct midi_controller_conf* get_midi_controller_conf(){ return current_conf_;};
	

		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){ UNUSED(btn_id); };
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id);

		void menu_enter();
		void menu_leave();
		void menu_update(uint32_t mst_ms, uint16_t mst_step);
		int  menu_on_push(uint8_t, uint8_t);
		int  menu_on_release(uint8_t, uint8_t);
		
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
};


#endif
