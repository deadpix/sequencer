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

#include "types.h"
#include "led_matrix.h"
#include "Bounce_array.h"
#include "clk.h"
#include "prog.h"
#include "menu.h"

#include "tempo.h"
#include "test_proj_one.h"
#include "test_proj_two.h"




menu  menu_ctrl;
prog *prog_arr[MATRIX_NR_COL];
prog *current_prog;
int nr_prog;

static tempo tempo_setting;
static test_proj_one p1;
static test_proj_two p2;




led_matrix 	current_lm;
led_matrix* lm_ptr;


clk mst_clk;
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

static void next_step(clk* c){
	uint32_t ms = c->clk_elapsed();
	if(ms > 0){
		uint16_t cnt = c->clk_get_step_cnt();
		if(cnt == 0)
			current_lm.clr_led_x(LED_RG_IDX,31);
		else 
			current_lm.toogle_led_x(LED_RG_IDX,(cnt-1));

		current_lm.toogle_led_x(LED_RG_IDX,c->clk_get_step_cnt());
	}
}

static void init_prog(){
	nr_prog = 0;
	led_matrix* menu_lmtx = menu_ctrl.get_menu_led_matrix();
	
	prog_arr[nr_prog] = &tempo_setting;
	set_prog_menu_entry(nr_prog, tempo_setting.clbk_menu_on_push, tempo_setting.clbk_menu_on_release, &tempo_setting);
	nr_prog++;
	
	prog_arr[nr_prog] = &p1;
	set_prog_menu_entry(nr_prog, p1.clbk_menu_on_push, p1.clbk_menu_on_release, &p1);
	menu_lmtx->set_led_x(LED_R_IDX, nr_prog * MATRIX_NR_ROW + 0);
	nr_prog++;
	
	prog_arr[nr_prog] = &p2;
	set_prog_menu_entry(nr_prog, p2.clbk_menu_on_push, p2.clbk_menu_on_release, &p2);
	menu_lmtx->set_led_x(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0);
	nr_prog++;

	// menu_ctrl MUST BE last
	prog_arr[nr_prog] = &menu_ctrl;

	for(int i=0;i<(nr_prog-1);i++){
		prog_arr[i]->set_menu_lm(menu_lmtx);
	}
	
	lm_ptr = p1.get_led_matrix();
	current_prog = prog_arr[1];
}


void setup(){
	setup_gui();
	init_matrix_btn();

	ui_timer.begin(upd_gui, 1000);
	btn_timer.begin(check_btn, 1000);
	midi_timer.begin(upd_midi, 8000);
	
	mst_clk.clk_set_max_step(32);

	init_prog();
	init_menu_btn(current_prog);

	init_midi();
}


void loop(){
	if(check_clk){
//		next_step(&mst_clk);
		check_clk = false;
	}
	if(btn_flag){
		scan(current_prog);
		scan_menu_btn();
	}
	midi_loop(midi_flag);
	if(midi_flag)
		midi_flag = false;
	
}