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

#include "types.h"
#include "led_matrix.h"
#include "Bounce_array.h"
#include "clk.h"
#include "prog.h"
#include "menu.h"

#include "track.h"
#include "step.h"
#include "sequencer.h"
#include "gui.h"

#include "tempo.h"
#include "test_proj_one.h"
#include "test_proj_two.h"



menu  menu_ctrl;
prog *prog_arr[MATRIX_NR_COL];
prog *current_prog;
int nr_prog;

static tempo tempo_setting;
static sequencer midi_seq;
static test_proj_one p1;
static test_proj_two p2;

led_matrix* lm_ptr;


clk* mst_clk;
LinkedList<clk *> slv_clks;
volatile bool check_clk;

uint8_t btn_col_idx;
volatile bool btn_flag;
volatile bool midi_flag;

IntervalTimer ui_timer;
IntervalTimer btn_timer;
IntervalTimer midi_timer;

static void upd_midi(){
	midi_flag = true;
}

static void check_btn(){
	if(!btn_flag)
		btn_flag = true;
}

static void upd_gui(){
	upd_shift_reg(lm_ptr);
	if(!check_clk)
		check_clk = true;
}

static void tempo_change_handler(uint32_t ms){
	noInterrupts();
	uint32_t tmp = ((ms*1000)/MIDI_SYNC_PPN);
	Serial.println(tmp);
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

static void init_one_prog(prog* p, int prog_id){
	prog_arr[prog_id] = p;
	p->set_prog_id(prog_id);
	set_prog_menu_entry(prog_id, p);	
}

static void init_all_prog(){
	nr_prog = 0;
	led_matrix* menu_lmtx = menu_ctrl.get_menu_led_matrix();
	
//	prog_arr[nr_prog] = &tempo_setting;
//	tempo_setting.set_prog_id(nr_prog);
//	set_prog_menu_entry(nr_prog, (prog *) &tempo_setting);
	init_one_prog((prog *) &tempo_setting, nr_prog);
	nr_prog++;
	
//	prog_arr[nr_prog] = &p1;
//	p1.set_prog_id(nr_prog);
//	set_prog_menu_entry(nr_prog, (prog *) &p1);
	init_one_prog((prog *) &p1, nr_prog);
	menu_lmtx->set_led_x(LED_R_IDX, nr_prog * MATRIX_NR_ROW + 0);
	nr_prog++;

//	prog_arr[nr_prog] = &p2;
//	p2.set_prog_id(nr_prog);
//	set_prog_menu_entry(nr_prog, (prog *) &p2);
	init_one_prog((prog *) &p2, nr_prog);
	menu_lmtx->set_led_x(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0);
	nr_prog++;

	init_one_prog((prog *) &midi_seq, nr_prog);
	menu_lmtx->set_led_x(LED_G_IDX, nr_prog * MATRIX_NR_ROW + 0);
	nr_prog++;
		
	// menu_ctrl MUST BE last
	prog_arr[nr_prog] = &menu_ctrl;

	for(int i=0;i<nr_prog;i++){
		prog_arr[i]->set_menu_lm(menu_lmtx);
	}
	
	lm_ptr = p1.get_led_matrix();
	current_prog = prog_arr[1];
	
	tempo_setting.init(tempo_change_handler);
	mst_clk = tempo_setting.get_mst_clk();
	mst_clk->clk_set_max_step(32);
	tempo_change_handler(mst_clk->clk_get_ms());

	init_midi_seq(&midi_seq);
	init_midi_controller(&p1);
//	sync_slv_clks(mst_clk);
}


void setup(){
	setup_oled();
	setup_gui();
	init_matrix_btn();

	ui_timer.begin(upd_gui, 1000);
	btn_timer.begin(check_btn, 1000);
//	midi_timer.begin(upd_midi, 8000);

	init_all_prog();
	init_midi();

	
	// MUST BE LAST...
	init_menu_btn(current_prog);

}


void loop(){
	uint32_t clk_res;
	if(check_clk){
		clk_res = tempo_setting.check_mst_clk();
		check_clk = false;
	}
	if(btn_flag){
		scan(current_prog);
		scan_menu_btn();
	}
	// if menu prog is running, call menu update function
	if(current_prog == prog_arr[nr_prog])
		menu_ctrl.menu_update();
	else
		current_prog->update_ui();
	
	midi_loop(midi_flag);
	if(midi_flag)
		midi_flag = false;
		
	midi_seq.check_clks(clk_res, mst_clk->clk_get_step_cnt());
}