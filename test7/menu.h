#ifndef __MENU_H__
#define __MENU_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "prog.h"
#include "led_matrix.h"





typedef boolean (* menu_clbk_def)(uint8_t, uint8_t, led_matrix*);
typedef led_matrix* (* menu_clbk_type)(void*, uint8_t, uint8_t, led_matrix*);

struct menu_clbk {
	menu_clbk_type clbk;
	void* obj_ptr;
};

class menu: public prog{
	private:
		led_matrix  menu_interface;
		led_matrix *next_interface;
		prog*		next_prog; 
		struct menu_clbk menu_clbk_arr[MATRIX_NR_COL];
	
	public:
		
		menu();
		~menu();
		
		led_matrix* get_menu_led_matrix();
		void set_menu_clbk(uint8_t prog_id, menu_clbk_type clbk, void *ptr);

		led_matrix* get_next_interface();
		prog* get_next_prog();
		void set_next_interface(led_matrix*);
		void set_next_prog(prog*);
		
		void menu_on_push(uint8_t btn_id);
		void menu_on_release(uint8_t btn_id);

		void on_push(void* ptr, uint8_t btn_id);
		void on_release(void* ptr, uint8_t btn_id);		
		led_matrix* get_led_matrix(void);
};

#endif