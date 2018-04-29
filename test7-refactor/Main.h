#ifndef __MAIN_H__
#define __MAIN_H__

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


class Main {
	private:
		void init_prog(prog*, int, char*);
		void init_all_prog(gui*);
		void init_sequencer();

	public:
		static menu menu_ctlr;
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

		Main(){};
		~Main(){};

		void init(gui*);
		void loop(void);
}

#endif
