#include "fct_loop_setting.h"

#define BASE10	10

static void upd_display(sequencer* seq, uint8_t val){
	char str[7];
	seq->prog::display_str(itoa(val,str,BASE10), 2);
}

void fct_loop_setting::init(sequencer* seq, char* name){
	_seq = seq;
	fct_clbk::set_fct_name(name);
}

void fct_loop_setting::on_push(uint8_t btn_id){
	upd_display(_seq, btn_id);

	track* t = _seq->get_current_track();
	uint8_t nr_step = t->get_max_step();

	if(nr_step == btn_id){
		t->get_led_matrix()->clr_n_restore(btn_id);	
	}
	else {
		t->get_led_matrix()->save_n_set(LED_G_IDX, btn_id);
		t->set_max_step(btn_id);
	}
}
void fct_loop_setting::on_release(uint8_t btn_id){
}
void fct_loop_setting::update_ui(uint32_t mst_ms, uint16_t mst_step){
}
void fct_loop_setting::on_start(){
	track* t = _seq->get_current_track();
	uint8_t nr_step = t->get_max_step();

	upd_display(_seq, nr_step);
	t->get_led_matrix()->save_n_set(LED_G_IDX, nr_step);
}
