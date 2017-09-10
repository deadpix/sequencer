#ifndef __PROG_H__
#define __PROG_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "led_matrix.h"

class prog {
	protected:
		uint8_t prog_id;
		led_matrix* _menu_lm;
	
	public:
		void set_prog_id(uint8_t);
		uint8_t get_prog_id(void);
		prog* get_prog(void);
		virtual led_matrix* get_led_matrix(void);
		
		led_matrix* get_menu_lm();
		void set_menu_lm(led_matrix*);
		
		virtual void menu_enter() = 0;
		virtual void menu_leave() = 0;
		virtual void menu_update() = 0;
		virtual int  menu_on_push(uint8_t, uint8_t) = 0;
		virtual int  menu_on_release(uint8_t, uint8_t) = 0;

		virtual void on_push(uint8_t btn_id) = 0;
		virtual void on_release(uint8_t btn_id) = 0;
};


#endif