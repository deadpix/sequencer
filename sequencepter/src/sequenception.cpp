#include <hw_debug.h>
#include "sequenception.h"
//#include "../interrupts.h"
#include <interrupts.h>
#include "midi_cst.h"

//static const uint8_t MIDI_DRUM_GM[8] = {37, 36, 42, 82, 40, 38, 46, 44};

menu sequenception::menu_ctrl;
prog* sequenception::prog_arr[MATRIX_NR_COL];
uint8_t sequenception::nr_prog;
tempo sequenception::tempo_setting;
sequencer sequenception::midi_seq;
midi_controller sequenception::mc;
midi_controller_param sequenception::mcp;
test_proj_two sequenception::p2;
seq_param sequenception::seq_param_ui;
fct_step sequenception::seq_option1;
fct_loop_setting sequenception::seq_option2;
prog* sequenception::current_prog;
led_matrix* sequenception::lm_ptr;
clk* sequenception::mst_clk;
uint32_t sequenception::clk_ms;
volatile uint8_t sequenception::track_upd;

void (*sequenception::fct_midi)(uint16_t, uint8_t, uint8_t);
void (*sequenception::fct_tempo_change)(uint32_t);


static void dummy_fct_midi(uint16_t arg1, uint8_t arg2, uint8_t arg3){
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
//	Serial.println("dummy midi function");
}
static void dummy_fct_tempo_change(uint32_t arg1){
	UNUSED(arg1);
//	Serial.println("dummy tempo change function");
}
sequenception::sequenception(){
	fct_midi = dummy_fct_midi;
	fct_tempo_change = dummy_fct_tempo_change;
}
sequenception::~sequenception(){
}
void sequenception::init_prog(prog* p, int prog_id, char* str){
	prog_arr[prog_id] = p;
	p->set_prog_id(prog_id);
	p->set_title(str);
	menu_ctrl.set_menu_prog_entry(prog_id, p);
}

void sequenception::init_sequencer(){
	seq_option1.init(&midi_seq, "step");
	seq_option2.init(&midi_seq, "looplen");
	
	midi_seq.add_fct(&seq_option1, SEQ_PARAM_STEP_RED_BTN_ID);
	midi_seq.add_fct(&seq_option1, SEQ_PARAM_STEP_BLUE_BTN_ID);
	midi_seq.add_fct(&seq_option1, SEQ_PARAM_STEP_GREEN_BTN_ID);
	midi_seq.add_fct(&seq_option2, SEQ_PARAM_LOOP_RED_BTN_ID);
	midi_seq.add_fct(&seq_option2, SEQ_PARAM_LOOP_BLUE_BTN_ID);
	midi_seq.add_fct(&seq_option2, SEQ_PARAM_LOOP_GREEN_BTN_ID);
	midi_seq.add_fct(&seq_option2, SEQ_PARAM_LOOP_RED_BLUE_BTN_ID);
	midi_seq.add_fct(&seq_option2, SEQ_PARAM_LOOP_RED_GREEN_BTN_ID);
	midi_seq.add_fct(&seq_option2, SEQ_PARAM_LOOP_BLUE_GREEN_BTN_ID);

	midi_seq.set_current_param(0);
	midi_seq.prog::display_str("step", 1);
	midi_seq.prog::set_param(&seq_param_ui);

	/* MUST BE called after sequencer initialization 	*/
	/* as parameters depends on sequencer options/fct 	*/
	/* initialization 					*/
	seq_param_ui.init(&midi_seq, mst_clk);
	seq_param_ui.param::set_prog(&midi_seq);
}

void sequenception::set_default_prog(prog* p){
	lm_ptr = p->get_led_matrix();
	current_prog = (prog *) &midi_seq;
	current_prog->display_title();	
}

void sequenception::init_all_prog(gui *g){

	nr_prog = 0;
	led_matrix* menu_lmtx = menu_ctrl.get_menu_led_matrix();
	
	init_prog((prog *) &tempo_setting, nr_prog, "Setting");
	nr_prog++;
	
	init_prog((prog *) &mc, nr_prog, "MIDIctl");
	menu_lmtx->save_n_set(LED_R_IDX, nr_prog * MATRIX_NR_ROW + 0, 0);
	nr_prog++;

	init_prog((prog *) &p2, nr_prog, "Prog 2");
	menu_lmtx->save_n_set(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0, 0);
	nr_prog++;

	init_prog((prog *) &midi_seq, nr_prog, "MIDIseq");
	menu_lmtx->save_n_set(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0, 0);
	nr_prog++;
		
	// menu_ctrl MUST BE last
	prog_arr[nr_prog] = &menu_ctrl;

	for(int i=0;i<nr_prog;i++){
		prog_arr[i]->set_menu_lm(menu_lmtx);
		prog_arr[i]->set_gui(g);
	}
	
	tempo_setting.init(fct_tempo_change, &midi_seq, true);
	mst_clk = tempo_setting.get_mst_clk();
	mst_clk->clk_set_max_step(NR_STEP);
	fct_tempo_change(mst_clk->clk_get_ms());

	init_midi_seq();
	init_midi_controller();
	init_sequencer();

	track_upd = 0x0;


	set_default_prog((prog *) &midi_seq);
}

void sequenception::init(gui *g){
	init_all_prog(g);
}
uint32_t sequenception::eval_mst_clk(){
	return tempo_setting.check_mst_clk_isr();
} 
void sequenception::loop(uint32_t ms){

	// disable the irq to get the first evt 
	DISABLE_IRQ();
	int nr_evt = evt_list.size();

	for(int i=0;i<nr_evt;i++){
		event* tmp = evt_list.shift();

		tmp->do_evt();
		delete tmp;
	}

	ENABLE_IRQ();
}

uint32_t sequenception::evt_master_tick(event** evt){
	uint32_t ms = eval_mst_clk();
	*evt = new master_clock_event(ms, this);
	(*evt)->set_event_id(EVT_MASTER_TICK);
	return ms;
}

void sequenception::do_isr(){
	event* evt = NULL;
	bool flg_mst_tick = false;	
	bool flg_next_step = false;	
	uint8_t tmp;

	
	for(int i=0;i<evt_list.size();i++){
		evt = evt_list.get(i);
		if(evt->get_event_id() == EVT_MASTER_TICK){
			flg_mst_tick = true;
		}
        else if(evt->get_event_id() == EVT_NEXT_STEP){
			flg_next_step = true;
        }
	}

	uint32_t ms = evt_master_tick(&evt);

    if(evt_list.size() == 0 || (ms > 0 && !flg_mst_tick)){
		evt_list.add(evt);
	} else {
		delete evt;
	}

    tmp = midi_seq.check_events(ms, mst_clk->clk_get_step_cnt(), &evt);
    if(evt_list.size() == 0 || tmp || !flg_next_step){
		evt_list.add(evt);
	} 
	else {
		delete evt;
	}



//	if(clk_ms == 0){
//		clk_ms = eval_mst_clk();
//		track_upd = midi_seq.check_events(clk_ms, mst_clk->clk_get_step_cnt());
//	}
//     	else {
//		Serial.println("do_isr() clk_ms > 0...");
//	}
	
}
void sequenception::init_midi_seq(){
	track* t;
	midi_seq.set_current_track(0);
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		t = midi_seq.get_track(i);
		t->set_out_id(1);
		t->set_all_step_note(MIDI_DRUM_GM[i]);
		t->init_hw_clbk(fct_midi);	
	}
}
void sequenception::init_midi_controller(){
	mc.init_hw_clbk(fct_midi);
	mc.prog::set_param(&mcp);
	mcp.init(&mc);
	mcp.param::set_prog(&mc);
}

master_clock_event::master_clock_event(uint32_t ms, sequenception* s){
	mst_clk_ms = ms;
	_sequenception = s;
}
void master_clock_event::do_evt(){
//	Serial.println("do evt");
	if(_sequenception->current_prog == _sequenception->prog_arr[_sequenception->nr_prog]){
		_sequenception->menu_ctrl.menu_update(mst_clk_ms,_sequenception->mst_clk->clk_get_step_cnt());
	}
	else {
		_sequenception->current_prog->update_ui(mst_clk_ms, _sequenception->mst_clk->clk_get_step_cnt());
	}	
}

