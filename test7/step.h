#ifndef __STEP_H__
#define __STEP_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"

#define GATE_UP			(1<<0)
#define GATE_DW			(1<<1)
#define GATE_ON			(GATE_UP_MASK | GATE_DW_MASK)
#define GATE_OFF		0x0

struct note {
	uint8_t  velocity;
	uint16_t pitch;
};

class step {
	private:
		uint8_t step_id;
		boolean flag_active;
		bool _linked;
		uint8_t _gate_bmp;
		uint8_t gate_len_per; // percentage
		uint32_t gate_len_ms; // percentage
		bool flg_gate;
//		gate _step_gate;
		elapsedMillis gate_elapsed;
	
	public:
		step();
		~step();
		struct note _note;
		
//		void step_init_gate(uint8_t (*hw_wr)(uint8_t, uint8_t));
	//	void check_gate(uint32_t);
		bool reset_gate();
		bool upd_gate();
		void set_step_gate_len(uint32_t ms, uint8_t len);
		void upd_step_gate_len(uint32_t);

		boolean is_step_active();
		void set_step_active();
		void clr_step_active();
		void set_step_up();
		void set_step_dw();
		void set_step_off();
//		void toogle_step();
		
		uint8_t get_step_gate_len();
		uint8_t get_step_id();
		
		void step_set_note(uint8_t, uint16_t);
		
		void set_step_id(uint8_t id);
		boolean step_status();
		void link_step();
		void unlink_step();
		bool is_step_linked();
};

#endif
