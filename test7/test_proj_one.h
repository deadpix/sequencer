#ifndef __TEST_PROJ_ONE_H__
#define __TEST_PROJ_ONE_H__

#include "prog.h"
#include "led_matrix.h"

class test_proj_one : public prog {
	private:
		led_matrix _lm;

	public:
		test_proj_one();
		~test_proj_one();
		
		void on_push(void* ptr, uint8_t btn_id);
		void on_release(void* ptr, uint8_t btn_id);
		
		static led_matrix* menu_clbk(void *, uint8_t, uint8_t, led_matrix*);
		led_matrix* update_menu(uint8_t, uint8_t, led_matrix*);
		led_matrix* get_led_matrix(void);
};


#endif