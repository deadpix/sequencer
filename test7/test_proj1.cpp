#include "test_proj1.h"

test_proj_one::test_proj_one(){
	
}

test_proj_one::~test_proj_one(){
	
}

led_matrix* test_proj_one::update_menu(uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	led_matrix* next; 
	for(int i=0; i<NUM_LED_COLUMNS; i++){
		if(i == opt_id){
			menu_matrix->set_led_x(LED_R_IDX, func_id * NUM_LED_ROWS + i);
			next = &_lm;
		}
		else {
			menu_matrix->clr_pixel(func_id * NUM_LED_ROWS + i);
			next = &_lm;
		}
	}	
	return next;
}

void test_proj_one::on_push(void* this_ptr, uint8_t btn_id){
	test_proj_one* myself = static_cast<test_proj_one *>(this_ptr);
}
void test_proj_one::on_release(void* this_ptr, uint8_t btn_id){
	test_proj_one* myself = static_cast<test_proj_one *>(this_ptr);
	_lm.toogle_led_x(LED_R_IDX, btn_id);
}
led_matrix* test_proj_one::menu_clbk(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	test_proj_one* myself = static_cast<test_proj_one *>(this_ptr);
	led_matrix* lm = myself->update_menu(func_id, opt_id, menu_matrix);
	return lm;
}