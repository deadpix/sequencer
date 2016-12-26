#ifndef __MENU_H__
#define __MENU_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "configuration.h"
#include "prog.h"
#include "led_matrix.h"

typedef boolean 	(* menu_clbk_def)(uint8_t, uint8_t, led_matrix*);
typedef led_matrix* (* menu_clbk_type)(void*, uint8_t, uint8_t, led_matrix*);

struct menu_clbk {
	menu_clbk_type clbk;
	void* obj_ptr;
};

class menu: public prog{
	private:
		led_matrix  menu_interface;
		led_matrix *next_interface;
//		led_matrix *prog_interface[NUM_LED_ROWS];
//		menu_clbk_def menu_clbk_arr[NUM_LED_ROWS];
		struct menu_clbk menu_clbk_arr[NUM_LED_ROWS];
	
	public:
		
		menu();
		~menu();
		
		led_matrix* get_menu_led_matrix();
//		void set_menu_clbk(uint8_t prog_id, menu_clbk_def clbk);
		void set_menu_clbk(uint8_t prog_id, menu_clbk_type clbk, void *ptr);
//		void init_prog_interface_pointer(uint8_t prog_id, led_matrix * lm);
		led_matrix* get_next_interface();
		void set_next_interface(led_matrix*);
		
		void menu_on_push(uint8_t btn_id);
		void menu_on_release(uint8_t btn_id);

		void on_push(void* ptr, uint8_t btn_id);
		void on_release(void* ptr, uint8_t btn_id);

//		static void callback_on_release(void*, uint8_t);
//		static void callback_on_push(void*, uint8_t);
		
};



#endif