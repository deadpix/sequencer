#include "sequencer.h"
#include <hw_debug.h>
sequencer::sequencer(){
	current_param_id = 0;
	for(int i=0;i<(MATRIX_NR_COL*MATRIX_NR_ROW);i++){
		fct_arr[i] = NULL;
	}
	_ls_ui.step_id = -1;
//	set_track_start(true);
}

sequencer::~sequencer(){
}

track* sequencer::get_track(uint8_t track_id){
	return &track_arr[track_id];
}

track* sequencer::get_current_track(){
	return current;
}

void sequencer::set_current_track(uint8_t track_id){
	current = &track_arr[track_id];
}

void sequencer::set_current_param(uint8_t id){
	current_param_id = id;
}
uint8_t sequencer::get_current_param(){
	return current_param_id;
}

void sequencer::add_fct(fct_clbk* fct, uint8_t idx){
	fct_arr[idx] = fct;
}
fct_clbk* sequencer::get_fct(uint8_t idx){
	return fct_arr[idx];
}

void sequencer::check_clks(uint32_t mst_ms, uint16_t mst_step){
	uint32_t res;
	track* t;
//	led_matrix *lm = current->get_led_matrix();
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		t = &track_arr[i];
		res = t->check_event(mst_ms, mst_step);
	}
//	if(res){
//		current->_step_animation.init_animation_n_save();
//	}
}
void sequencer::set_track_start(bool play){
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		track_arr[i].set_play(play);
	}
}
void sequencer::reset_all(){
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		track_arr[i].step_reset();
	}
}

void sequencer::menu_enter(){
}
void sequencer::menu_leave(){
}
void sequencer::menu_update(){
}


int sequencer::menu_on_push(uint8_t func_id, uint8_t opt_id){	
	int ret = 1;
	prog::display_title();
	return ret;
}
int sequencer::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	for(int i=0; i<MATRIX_NR_COL; i++){
		if(i == opt_id){
			get_menu_lm()->set_led_x(LED_B_IDX, func_id * MATRIX_NR_ROW + i);
			current = get_track(i);
		}
		else {
			get_menu_lm()->clr_led_x(LED_B_IDX, func_id * MATRIX_NR_ROW + i);
		}
	}	
	return ret;
}

void sequencer::on_push(uint8_t btn_id){
//	current->on_push(btn_id);
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->on_push(btn_id);
	}
}
void sequencer::on_long_push(uint8_t btn_id){
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->on_long_push(btn_id);
	}
}
void sequencer::on_release(uint8_t btn_id){
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->on_release(btn_id);
	}
}
void sequencer::on_long_release(uint8_t btn_id){
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->on_long_release(btn_id);
	}
}

void sequencer::update_ui(uint32_t mst_ms, uint16_t mst_step){
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->update_ui(mst_ms,mst_step);
	}
}
led_matrix* sequencer::get_led_matrix(){
	current->get_led_matrix();
}
