#ifndef __MENU_H__
#define __MENU_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "led_matrix.h"
#include "prog.h"


typedef boolean (* menu_clbk_def)(uint8_t, uint8_t, led_matrix*);
typedef led_matrix* (* menu_clbk_type)(/*void*,*/ uint8_t, uint8_t/*, led_matrix**/);
typedef void (* menu_entry_type)(void);

struct menu_clbk {
	menu_clbk_type clbk_on_push;
	menu_clbk_type clbk_on_release;
	
	menu_entry_type enter;
	menu_entry_type leave;
	menu_entry_type update;
	
	void* obj_ptr;
};

class menu: public prog {
	private:
		led_matrix  menu_interface;
		prog*		next_prog; 
		struct menu_clbk menu_clbk_arr[MATRIX_NR_COL];
		prog*		prog_arr[MATRIX_NR_COL];
	
	public:
		
		menu();
		~menu();
		
		led_matrix* get_menu_led_matrix();
		void set_menu_prog_entry(uint8_t prog_id, prog* p);

		prog* get_next_prog();
		void set_next_prog(prog*);
		
		void menu_enter();
		void menu_leave();
		void menu_update();

		int menu_on_push(uint8_t, uint8_t);
		int menu_on_release(uint8_t, uint8_t);	
	
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);		
		led_matrix* get_led_matrix(void);
		void update_ui();
};

#endif