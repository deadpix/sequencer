#ifndef __TEST_PROJ_ONE_H__
#define __TEST_PROJ_ONE_H__

#include <LinkedList.h>

#include "prog.h"
#include "led_matrix.h"
#include "led_toogle.h"

class test_proj_one : public prog {
	private:
		led_matrix _lm;
		led_toogle _btn_animation;
		LinkedList<led_toogle *> _btn_animation_list;
		
	public:
		test_proj_one();
		~test_proj_one();

		led_matrix* get_led_matrix(void);
		void init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t));

		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){};
		void on_release(uint8_t btn_id);

		void menu_enter();
		void menu_leave();
		void menu_update();
		int  menu_on_push(uint8_t, uint8_t);
		int  menu_on_release(uint8_t, uint8_t);
		
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
};


#endif
