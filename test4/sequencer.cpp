#include "sequencer.h"

sequencer::sequencer(){
}

sequencer::~sequencer(){
}

seq* sequencer::get_seq(uint8_t seq_id){
	return &seq_arr[seq_id];
}

seq* sequencer::get_current_seq(){
	return current;
}

void sequencer::set_current_seq(uint8_t seq_id){
	current = &seq_arr[seq_id];
}

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

void sequencer::update_gui(){
	current->update_gui();
}

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
