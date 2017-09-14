#ifndef __TRACK_H__
#define __TRACK_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "step.h"
#include "clk.h"
#include "led_matrix.h"
#include "menu.h"
#include "prog.h"
#include "led_toogle.h"

class track {
	private:
		uint8_t	curr_step_id;
//		uint8_t max_step;
		uint8_t track_len;
		clk _c;
		step arr_step[NR_STEP];
		elapsedMillis elapsed_ms;
		led_matrix _lm;
		uint8_t output_id;
		boolean mute_flg;
		led_toogle _step_animation;
		
	public:
		track();
		~track();
		
		led_matrix * get_led_matrix();
		clk* get_clk();
		boolean is_step_on(uint8_t id);
		boolean next_step();
		// boolean is_curr_step_active(uint32_t ms);
		uint8_t get_current_step();
		void toogle_step_x(uint8_t id);

		void check_event(boolean, clk *c);
//		void init_hw_clbk(void (*hw_fct)(uint8_t, boolean), uint8_t);

		void init_menu();
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);

		void mute();
		void unmute();
		void toogle_mute();
		
		void update_ui();
};


#endif