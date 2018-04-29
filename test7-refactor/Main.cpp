#include "Main,h"

void Main::init_prog(prog* p, int prog_id, char* str){
	prog_arr[prog_id] = p;
	p->set_prog_id(prog_id);
	p->set_title(str);
	menu_ctrl.set_menu_prog_entry(prog_id, p);
}

void Main::init_sequencer(){
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

void Main::init_all_prog(gui *g){
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
		prog_arr[i]->set_gui(g);
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

}

void Main::init(gui *g){
	init_all_prog(g);
}
void Main::loop(){

}
