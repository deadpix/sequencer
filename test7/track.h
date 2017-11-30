#ifndef __TRACK_H__
#define __TRACK_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "led_matrix.h"
#include "menu.h"
#include "prog.h"
#include "led_toogle.h"
#include "step.h"
#include "clk.h"

class track {
	private:
		uint16_t curr_step_id;
		uint8_t _max_step;
		uint8_t _track_id;
		uint8_t _out_id;
		
		uint8_t track_len;
		clk _c;
		elapsedMillis elapsed_ms;
		led_matrix _lm;
		uint8_t output_id;
		boolean mute_flg;
		led_toogle _step_animation;
		
	public:
		step arr_step[NR_STEP];
		track();
		~track();
		
		led_matrix * get_led_matrix();
		clk* get_clk();

		void set_max_step(uint8_t max);
		uint8_t get_max_step();
		
		void set_track_id(uint8_t id);
		uint8_t get_track_id();

		void set_out_id(uint8_t id);
		uint8_t get_out_id();

		void set_all_step_note(uint16_t);
		void set_step_note(uint16_t, uint8_t );
		
		boolean is_step_on(uint8_t id);
		boolean next_step();
		// boolean is_curr_step_active(uint32_t ms);
		uint8_t get_current_step();
		void toogle_step_x(uint8_t id);

		uint32_t check_event(uint32_t, uint16_t/*boolean, clk *c*/);
		void init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t));

		void init_menu();
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);

		void mute();
		void unmute();
		void toogle_mute();
		
		void update_ui();
};


#endif