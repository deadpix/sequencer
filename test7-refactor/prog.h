#ifndef __PROG_H__
#define __PROG_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

class param;

#include "types.h"
#include "led_matrix.h"
#include "gui.h"

class prog {
	protected:
		uint8_t prog_id;
		led_matrix* _menu_lm;
		led_matrix* _param_lm;
		gui* 	_gui;
		char* 	_title;
		param* 	_param;

	public:
		void display_title();
		void display_str(char*, uint8_t);

		void set_prog_id(uint8_t);
		uint8_t get_prog_id(void);
		prog* get_prog(void);
		virtual led_matrix* get_led_matrix(void);
		
		param* get_param();
		void set_param(param*);
		
		led_matrix* get_menu_lm();
		void set_menu_lm(led_matrix*);
		
		void set_gui(gui*);
		void set_title(char*);

		virtual void menu_enter() = 0;
		virtual void menu_leave() = 0;
		virtual void menu_update() = 0;
		virtual int  menu_on_push(uint8_t, uint8_t) = 0;
		virtual int  menu_on_release(uint8_t, uint8_t) = 0;

		virtual void on_push(uint8_t btn_id) = 0;
		virtual void on_long_push(uint8_t btn_id) = 0;
		virtual void on_release(uint8_t btn_id) = 0;
		virtual void on_long_release(uint8_t btn_id) = 0;
		virtual void update_ui(uint32_t mst_ms, uint16_t mst_step) = 0;

//		virtual void select_param(uint8_t);
};

#include "param.h"

#endif
