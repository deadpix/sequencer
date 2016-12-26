#ifndef __SEQ_H__
#define __SEQ_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "configuration.h"
#include "step.h"
#include "clock.h"
#include "led_matrix.h"
#include "menu.h"
#include "prog.h"

class seq {
	private:
		uint8_t	curr_step_id;
//		uint8_t max_step;
		uint8_t seq_len;
		clock clk;
		step arr_step[NR_STEP];
		elapsedMillis elapsed_ms;
		led_matrix _lm;
		uint8_t output_id;
		boolean mute_flg;
		
	public:
		seq();
		~seq();
		
		led_matrix * get_led_matrix();
		clock* get_clk();
		boolean is_step_on(uint8_t id);
		boolean next_step();
		boolean is_curr_step_active(uint32_t ms);
		uint8_t get_current_step();
		void toogle_step_x(uint8_t id);
//		void reset();
		void check_event(boolean, clock*);
		void init_hw_clbk(void (*hw_fct)(uint8_t, boolean), uint8_t);
		void init_menu();
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);
//		menu_clbk_type get_menu_clbk();
//		led_matrix* update_menu(uint8_t, uint8_t, led_matrix*);
		void mute();
		void unmute();
		void toogle_mute();
		
		static void callback_on_release(void*, uint8_t);
		static void callback_on_push(void*, uint8_t);
//		static led_matrix* menu_clbk(void *, uint8_t, uint8_t, led_matrix*);
		
		void update_gui();
//		static void hw_write(uint8_t, boolean);
};


#endif