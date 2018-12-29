#ifndef __SEQUENCEPTION_H__
#define __SEQUENCEPTION_H__

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif

#include <stdint.h>

#include "types.h"

#include "prog.h"
#include "menu.h"
#include "led_matrix.h"
#include "tempo.h"
#include "sequencer.h"
#include "test_proj_one.h"
#include "test_proj_two.h"
#include "gui.h"
#include "clk.h"
#include "sequencer/fct_step.h"
#include "sequencer/fct_loop_setting.h"
#include "seq_param.h"



class sequenception {
	private:
		void init_prog(prog*, int, char*);
		void init_all_prog(gui*);
		void init_sequencer();
		void init_midi_seq();
		void init_midi_controller();
		void set_default_prog(prog*);

	public:
		static menu menu_ctrl;
		static prog *prog_arr[MATRIX_NR_COL];
		static uint8_t nr_prog;

		static tempo tempo_setting;
		static sequencer midi_seq;
		static test_proj_one p1;
		static test_proj_two p2;

		static seq_param seq_param_ui;
		static fct_step seq_option1;
		static fct_loop_setting seq_option2;

		static prog* current_prog;
		static led_matrix* lm_ptr;
		static clk* mst_clk;

		static void (*fct_midi)(uint16_t, uint8_t, uint8_t);
		static void (*fct_tempo_change)(uint32_t);

		sequenception();
		~sequenception();

		void init(gui*);
		uint32_t eval_mst_clk();
		void loop(uint32_t);
};

#endif
