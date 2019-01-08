/*
 *	Copyright (c) 2017 Vincent "deadpix" Dupre
 *
 *	Author: Vincent Dupre (vdupre@gmail.com)
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */ 
#include <i2c_t3.h>
#include <Adafruit_MCP23017.h>
#include <LinkedList.h>
#include <Adafruit_NeoTrellis.h>
#include <hw_debug.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "configuration.h"

#include "src/types.h"
#include "src/sequenception.h"
#include "src/led_matrix.h"
#include "Bounce_array.h"
#include "src/clk.h"
#include "src/prog.h"
#include "src/menu.h"

#include "src/track.h"
#include "src/step.h"
#include "src/sequencer.h"
#include "src/gui.h"

#include "src/tempo.h"
#include "src/test_proj_one.h"
#include "src/test_proj_two.h"
#include "src/sequencer/fct_step.h"
#include "src/sequencer/fct_loop_setting.h"
#include "src/seq_param.h"


static sequenception sequenception;

gui	 *gui_ctrl;
//menu  menu_ctrl;
//prog *prog_arr[MATRIX_NR_COL];
//prog *current_prog;
//int nr_prog;

//static tempo tempo_setting;
//static sequencer midi_seq;
//static seq_param seq_param_ui;
//static fct_step seq_option1;
//static fct_loop_setting seq_option2;

//static test_proj_one p1;
//static test_proj_two p2;

//led_matrix* lm_ptr;


//clk* mst_clk;
volatile bool check_clk;

uint8_t btn_col_idx;
volatile bool btn_flag;
volatile bool midi_flag;

IntervalTimer ui_timer;
IntervalTimer btn_timer;
IntervalTimer midi_timer;

bool flag_btn_active;

static void upd_midi(){
	midi_flag = true;
}

static void check_btn(){
	if(!btn_flag)
		btn_flag = true;
}

#if HW_SHIFT_REG == 1 
static void upd_gui(){
	upd_shift_reg(sequenception.lm_ptr);
	if(!check_clk)
		check_clk = true;
}
#endif

static void tempo_change_handler(uint32_t ms){
	noInterrupts();
	uint32_t tmp = ((ms*1000)/MIDI_SYNC_PPN);
//	Serial.println(tmp);
	midi_timer.begin(upd_midi, tmp);
	interrupts();
}
/*
void init_clk(clk* c){
	slv_clks.add(c);
}

static void sync_slv_clks(clk* mst){
	uint8_t nr_clk = slv_clks.size();
	uint32_t ms = mst->clk_get_ms();
	for(int i=0;i<nr_clk;i++){
		clk* tmp = slv_clks.get(i);
		tmp->clk_sync_intern(ms);
	}
}
*/


//static void init_sequencer(){
//	seq_option1.init(&midi_seq, "step");
//	seq_option2.init(&midi_seq, "looplen");
//	
//	midi_seq.add_fct(&seq_option1, 0);
//	midi_seq.add_fct(&seq_option2, 1);
//
//	midi_seq.set_current_param(0);
//	midi_seq.prog::display_str("step", 1);
//	midi_seq.prog::set_param(&seq_param_ui);
//
//	/* MUST BE called after sequencer initialization 	*/
//	/* as parameters depends on sequencer options/fct 	*/
//	/* initialization 									*/
//	seq_param_ui.init(&midi_seq, mst_clk);
//	seq_param_ui.param::set_prog(&midi_seq);
//
//}
/*
static void init_one_prog(prog* p, int prog_id, char *str){
	prog_arr[prog_id] = p;
	p->set_prog_id(prog_id);
	p->set_title(str);
	set_prog_menu_entry(prog_id, p);	
}
*/
/*
static void init_all_prog(){
	nr_prog = 0;
	led_matrix* menu_lmtx = menu_ctrl.get_menu_led_matrix();
	
	init_one_prog((prog *) &tempo_setting, nr_prog, "Setting");
	nr_prog++;
	
	init_one_prog((prog *) &p1, nr_prog, "MIDIctl");
	menu_lmtx->set_led_x(LED_R_IDX, nr_prog * MATRIX_NR_ROW + 0);
	nr_prog++;

	init_one_prog((prog *) &p2, nr_prog, "Prog 2");
	menu_lmtx->set_led_x(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0);
	nr_prog++;

	init_one_prog((prog *) &midi_seq, nr_prog, "MIDIseq");
	menu_lmtx->set_led_x(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0);
	nr_prog++;
		
	// menu_ctrl MUST BE last
	prog_arr[nr_prog] = &menu_ctrl;

	for(int i=0;i<nr_prog;i++){
		prog_arr[i]->set_menu_lm(menu_lmtx);
		prog_arr[i]->set_gui(gui_ctrl);
	}
	
	lm_ptr = p1.get_led_matrix();
	current_prog = prog_arr[1];
	current_prog->display_title();

	
	tempo_setting.init(tempo_change_handler, &midi_seq);
	mst_clk = tempo_setting.get_mst_clk();
	mst_clk->clk_set_max_step(NR_STEP);
	tempo_change_handler(mst_clk->clk_get_ms());

	init_midi_seq(&midi_seq);
	init_midi_controller(&p1);
	init_sequencer();
//	sync_slv_clks(mst_clk);
}
*/

void setup(){
	// Hardware init procedure
	gui_ctrl = setup_oled();
	setup_matrix();
//	setup_gui();
//	init_matrix_btn();
	init_midi();
	sequenception.fct_midi = midi_note_on;
	sequenception.fct_tempo_change = tempo_change_handler;

	// timer
#if HW_SHIFT_REG == 1 
	ui_timer.begin(upd_gui, 1000);
#endif
	btn_timer.begin(check_btn, 1000);
//	midi_timer.begin(upd_midi, 8000);

	// data initialization
//	init_all_prog();
	sequenception.init(gui_ctrl);
	
	// MUST BE LAST...
	init_menu_btn(sequenception.current_prog);

}


void loop(){
	uint32_t clk_res = 0;
	if(check_clk){
//		clk_res = sequenception.tempo_setting.check_mst_clk();
		clk_res = sequenception.eval_mst_clk();
		check_clk = false;
	}
	if(btn_flag){
		scan(sequenception.current_prog);
		scan_menu_btn();
		scan_param_btn();
	}
	// if menu prog is running, call menu update function
	if(sequenception.current_prog == sequenception.prog_arr[sequenception.nr_prog]){
		sequenception.menu_ctrl.menu_update();
	} 
	else {
		sequenception.current_prog->update_ui(clk_res, sequenception.mst_clk->clk_get_step_cnt());
	}

	midi_loop(midi_flag);
	if(midi_flag)
		midi_flag = false;
		
//	midi_seq.check_clks(clk_res, sequenception.mst_clk->clk_get_step_cnt());
	sequenception.loop(clk_res);
}
