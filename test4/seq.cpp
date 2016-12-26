#include "seq.h"

static void (*hw_dWrite)(uint8_t, boolean);

seq::seq(){
	curr_step_id = 0;
	seq_len = 16;
	elapsed_ms = 0;
	
	for(int i=0; i<NR_STEP;i++){
		arr_step[i].set_step_id(i);
	}
}

seq::~seq(){
}

void seq::init_hw_clbk(void (*hw_fct)(uint8_t, boolean), uint8_t id){
	hw_dWrite = hw_fct;
	output_id = id;
}

led_matrix* seq::get_led_matrix(){
	return &_lm;
}

boolean seq::next_step(){
	curr_step_id = (curr_step_id + 1) % NR_STEP;
	elapsed_ms = 0;
	return arr_step[curr_step_id].is_step_active();
}

boolean seq::is_curr_step_active(uint32_t ms){
	uint32_t limit = (arr_step[curr_step_id].get_step_gate_len() * ms) / 100; 
	if( (elapsed_ms > limit) || !(is_step_on(curr_step_id)) )
		return false;
	else
		return true;
}

uint8_t seq::get_current_step(){
	return curr_step_id;
}

clock* seq::get_clk(){
	return &clk;
}


void seq::toogle_step_x(uint8_t id){
	arr_step[id].toogle_step();
}

boolean seq::is_step_on(uint8_t id){
	return arr_step[id].step_status();
}

void seq::mute(){
	mute_flg = true;
}
void seq::unmute(){
	mute_flg = false;
}

void seq::toogle_mute(){
	mute_flg = !mute_flg;
}

void seq::check_event(boolean master_clk_flg, clock* master_clk){
	boolean evt_flg = clk.clock_update(master_clk_flg, master_clk);
	boolean step_state;
	if(evt_flg){
		step_state = next_step();
//		_lm.set_pixel(curr_step_id);
	} else {
		step_state = is_curr_step_active(clk.getInterval());
	}

	if(!mute_flg) hw_dWrite(output_id, step_state);
}

void seq::on_push(uint8_t btn_id){
	
}
void seq::on_release(uint8_t btn_id){
	_lm.toogle_pixel_status( (btn_id / NUM_LED_ROWS),(btn_id % NUM_LED_COLUMNS));
	toogle_step_x(btn_id);
}

void seq::callback_on_push(void * this_ptr, uint8_t btn_id){
	seq* myself = static_cast<seq *>(this_ptr);
	myself->on_push(btn_id);
}
void seq::callback_on_release(void * this_ptr, uint8_t btn_id){
	seq* myself = static_cast<seq *>(this_ptr);
	myself->on_release(btn_id);
}

//led_matrix* seq::update_menu(uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
//	for(int i=0; i<NUM_LED_COLUMNS; i++){
//		if(i == opt_id){
//			menu_matrix->set_pixel(func_id * NUM_LED_ROWS + i);		
//		}
//		else {
//			menu_matrix->clr_pixel(func_id * NUM_LED_ROWS + i);
//		}
//	}
//	return &_lm;
//}
//led_matrix* seq::menu_clbk(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
//	seq* myself = static_cast<seq *>(this_ptr);
//	led_matrix* lm = myself->update_menu(func_id, opt_id, menu_matrix);
//
//	return lm;
//}

void seq::update_gui(){
	clk.clock_upd_gui(curr_step_id, get_led_matrix(), is_step_on(curr_step_id));
}

//menu_clbk_def seq::get_menu_clbk(){
//	return menu_clbk;
//}