#include "sequencer.h"

sequencer::sequencer(){
	current_param_id = 0;
	for(int i=0;i<(MATRIX_NR_COL*MATRIX_NR_ROW);i++){
		fct_arr[i] = NULL;
	}
//	_transport.play = 0x0;
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
	track* t;
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		t = &track_arr[i];
		t->check_event(mst_ms, mst_step);
	}
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
//	current->on_release(btn_id);
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
//	current->update_ui();
	if(fct_arr[current_param_id])
		fct_arr[current_param_id]->update_ui(mst_ms,mst_step);
}
led_matrix* sequencer::get_led_matrix(){
	current->get_led_matrix();
}


/*
void sequencer::on_push(void* this_ptr, uint8_t btn_id){
	sequencer* myself = static_cast<sequencer *>(this_ptr);
	seq* tmp = myself->current;
	tmp->on_push(btn_id);
}
void sequencer::on_release(void* this_ptr, uint8_t btn_id){
	sequencer* myself = static_cast<sequencer *>(this_ptr);
	seq* tmp = myself->current;
	tmp->on_release(btn_id);
}

void sequencer::update_out(clock* master){
	boolean clock_flag;
	for(int i=0;i<SEQUENCER_NR_PART;i++){
		seq *s = get_seq(i);
		s->check_event(clock_flag, master);
	}
}
*/
/*
void sequencer::update_gui(){
	current->update_gui();
}
*/
/*
led_matrix* sequencer::update_menu(uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	led_matrix* next; 
	for(int i=0; i<NUM_LED_COLUMNS; i++){
		if(i == opt_id){
			menu_matrix->set_pixel(func_id * NUM_LED_ROWS + i);		
			next = seq_arr[i].get_led_matrix();
			current = get_seq(i);
		}
		else {
			menu_matrix->clr_pixel(func_id * NUM_LED_ROWS + i);
		}
	}	
	return next;
}

led_matrix* sequencer::menu_clbk(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	sequencer* myself = static_cast<sequencer *>(this_ptr);
	led_matrix* lm = myself->update_menu(func_id, opt_id, menu_matrix);
	return lm;
}
*/
