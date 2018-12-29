#include <hw_debug.h>
#include "sequenception.h"

static const uint8_t MIDI_DRUM_GM[8] = {37, 36, 42, 82, 40, 38, 46, 44};

menu sequenception::menu_ctrl;
prog* sequenception::prog_arr[MATRIX_NR_COL];
uint8_t sequenception::nr_prog;
tempo sequenception::tempo_setting;
sequencer sequenception::midi_seq;
test_proj_one sequenception::p1;
test_proj_two sequenception::p2;
seq_param sequenception::seq_param_ui;
fct_step sequenception::seq_option1;
fct_loop_setting sequenception::seq_option2;
prog* sequenception::current_prog;
led_matrix* sequenception::lm_ptr;
clk* sequenception::mst_clk;

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
	
	midi_seq.add_fct(&seq_option1, 0);
	midi_seq.add_fct(&seq_option2, 1);

	midi_seq.set_current_param(0);
	midi_seq.prog::display_str("step", 1);
	midi_seq.prog::set_param(&seq_param_ui);

	/* MUST BE called after sequencer initialization 	*/
	/* as parameters depends on sequencer options/fct 	*/
	/* initialization 									*/
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
	
	init_prog((prog *) &p1, nr_prog, "MIDIctl");
//	menu_lmtx->set_led_x(LED_R_IDX, nr_prog * MATRIX_NR_ROW + 0);
	menu_lmtx->save_n_set(LED_R_IDX, nr_prog * MATRIX_NR_ROW + 0, 0);
	nr_prog++;

	init_prog((prog *) &p2, nr_prog, "Prog 2");
//	menu_lmtx->set_led_x(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0);
	menu_lmtx->save_n_set(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0, 0);
	nr_prog++;

	init_prog((prog *) &midi_seq, nr_prog, "MIDIseq");
//	menu_lmtx->set_led_x(LED_B_IDX, nr_prog * MATRIX_NR_ROW + 0);
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

	set_default_prog((prog *) &midi_seq);
}

void sequenception::init(gui *g){
	init_all_prog(g);
}
uint32_t sequenception::eval_mst_clk(){
	return tempo_setting.check_mst_clk();
} 
void sequenception::loop(uint32_t ms){
	// if menu prog is running, call menu update function
	if(current_prog == prog_arr[nr_prog]){
		menu_ctrl.menu_update();
	}
	else {
		current_prog->update_ui(ms, mst_clk->clk_get_step_cnt());
	}
	midi_seq.check_clks(ms, mst_clk->clk_get_step_cnt());
}
void sequenception::init_midi_seq(){
	track* t;
	midi_seq.set_current_track(0);
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		t = midi_seq.get_track(i);
		t->set_out_id(i+1);
		t->set_all_step_note(MIDI_DRUM_GM[i]);
		t->init_hw_clbk(fct_midi);	
	}
}
void sequenception::init_midi_controller(){
	p1.init_hw_clbk(fct_midi);
}
