#ifndef __TEST_PROJ_TWO_H__
#define __TEST_PROJ_TWO_H__

#include "prog.h"
#include "led_matrix.h"

class test_proj_two : public prog {
	private:
		led_matrix _lm;

	
	public:
		test_proj_two();
		~test_proj_two();

		led_matrix* get_led_matrix(void);
		
		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){};
		void on_release(uint8_t btn_id);
		void update_ui(uint32_t mst_ms, uint16_t mst_step);

		void menu_enter();
		void menu_leave();
		void menu_update();
		int  menu_on_push(uint8_t, uint8_t);
		int  menu_on_release(uint8_t, uint8_t);
};

#endif
