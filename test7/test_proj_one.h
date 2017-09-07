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

		led_matrix* get_led_matrix(void);
		
		void on_push(/*void* ptr, */uint8_t btn_id);
		void on_release(/*void* ptr, */uint8_t btn_id);

		void menu_enter();
		void menu_leave();
		void menu_update();
		int menu_on_push(uint8_t, uint8_t/*, led_matrix**/);
		int menu_on_release(uint8_t, uint8_t/*, led_matrix**/);	

//		static led_matrix* clbk_menu_on_push(void *, uint8_t, uint8_t, led_matrix*);
//		static led_matrix* clbk_menu_on_release(void *, uint8_t, uint8_t, led_matrix*);

//		led_matrix* update_menu(uint8_t, uint8_t, led_matrix*);
};


#endif