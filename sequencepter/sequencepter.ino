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
//#include <Wire.h>
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
#include "src/perf.h"

#define TRACE_PERF 1

#if TRACE_PERF == 1
static perf p;
#endif


static sequenception sequenception;
param* param_ptr;

gui	 *gui_ctrl;
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

static void upd_gui(){
#if HW_SHIFT_REG == 1 
	upd_shift_reg(sequenception.lm_ptr);
//	if(!check_clk){
//		check_clk = true;
//	}
#endif
	sequenception.do_isr();	
}

static void tempo_change_handler(uint32_t ms){
	noInterrupts();
	uint32_t tmp = ((ms*1000)/MIDI_SYNC_PPN);
	midi_timer.begin(upd_midi, tmp);
	interrupts();
}

void setup(){
	// Hardware init procedure
	Serial.begin(9600);

	gui_ctrl = setup_oled();
	setup_matrix();
	init_midi();
	sequenception.fct_midi = midi_note_on;
	sequenception.fct_tempo_change = tempo_change_handler;

	// timer
	ui_timer.begin(upd_gui, 10000);
	btn_timer.begin(check_btn, 10000);
//	midi_timer.begin(upd_midi, 8000);

#if CMD_BTN_MATRIX == 1
	setup_cmd_btn_matrix();	
#endif

	// data initialization
	sequenception.init(gui_ctrl);
	switch_matrix_ui(sequenception.lm_ptr, sequenception.lm_ptr);
	
	// MUST BE LAST...
	init_menu_btn(sequenception.current_prog);

}


void loop(){

//#if TRACE_PERF == 1
//	p.start_ms_counter();
//#endif

	uint32_t clk_res = 0;
//	if(check_clk){
//		clk_res = sequenception.tempo_setting.check_mst_clk();
//		clk_res = sequenception.eval_mst_clk();
//		check_clk = false;
//	}



	if(btn_flag){
		PERF_START(1)
		scan(sequenception.current_prog);
		PERF_END(1, 500)
#if CMD_BTN_MATRIX == 0
		scan_menu_btn();
		scan_param_btn();
#else
		scan_cmd_btn_matrix();
#endif
		btn_flag = false;
	}


	midi_loop(midi_flag);
	if(midi_flag)
		midi_flag = false;
		
	sequenception.loop(clk_res);



}
