#ifndef __STEP_H__
#define __STEP_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "configuration.h"

class step {
	private:
		uint8_t step_id;
		boolean flag_active;
		uint8_t gate_len; // percentage
		
	public:
		step();
		~step();
		
		boolean is_step_active();
		void set_step_active();
		void clr_step_active();
		void toogle_step();
		void set_step_gate_len(uint8_t len);
		uint8_t get_step_gate_len();
		uint8_t get_step_id();
		void set_step_id(uint8_t id);
		boolean step_status();
		
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);

};

#endif