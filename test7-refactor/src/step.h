#ifndef __STEP_H__
#define __STEP_H__

#include <stdint.h>
//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif

#include "types.h"
#include "clk.h"

#define GATE_UP			(1<<0)
#define GATE_DW			(1<<1)
#define GATE_ON			(GATE_UP | GATE_DW)
#define GATE_OFF		0x0

struct note {
	uint8_t  velocity;
	uint16_t pitch;
};

class step {
	private:
		uint8_t step_id;
		bool flag_active;
		bool _linked;
		uint8_t _gate_bmp;
		uint8_t gate_len_per; // percentage
		uint32_t gate_len_ms; // percentage
		bool _flg_gate;
		elapsedMillis gate_elapsed;
		
		step* _next;
		clk* _c;
	
		uint8_t _color;

	public:
		step();
		~step();
		struct note _note;
		struct clk_def _clk_def;
		uint8_t _step_ui_id;
		

//		void step_init_gate(uint8_t (*hw_wr)(uint8_t, uint8_t));
	//	void check_gate(uint32_t);
		bool reset_gate();
		bool upd_gate();
		void set_step_gate_len(uint32_t ms, uint8_t len);
		void upd_step_gate_len(uint32_t);

		bool is_step_active();
		void set_step_active();
		void clr_step_active();
		void set_step_up();
		void set_step_dw();
		void set_step_off();
//		void toogle_step();
		
		void set_next_step(step*);
		step* get_next_step();

		void set_step_color(uint8_t);
		uint8_t get_step_color();


		void set_clk(clk*);
		clk* get_clk();

		uint8_t get_step_gate_len();
		uint8_t get_step_id();
		
		void step_set_note(uint8_t, uint16_t);
		
		void set_step_id(uint8_t id);
		bool step_status();
		void link_step();
		void unlink_step();
		bool is_step_linked();
};

#endif
