#include "test_proj_two.h"

test_proj_two::test_proj_two(){
	
}

test_proj_two::~test_proj_two(){
	
}

led_matrix* test_proj_two::get_led_matrix(){
	return &_lm;
}

void test_proj_two::menu_enter(){
}
void test_proj_two::menu_leave(){
}
void test_proj_two::menu_update(){
}

void test_proj_two::on_push(uint8_t btn_id){
}
void test_proj_two::on_release(uint8_t btn_id){
	_lm.toogle_led_x(LED_B_IDX, btn_id);
}

int test_proj_two::menu_on_push(uint8_t func_id, uint8_t opt_id){	
	int ret = 1;
	prog::display_title();
	return ret;
}
int test_proj_two::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	for(int i=0; i<MATRIX_NR_COL; i++){
		if(i == opt_id){
			get_menu_lm()->set_led_x(LED_B_IDX, func_id * MATRIX_NR_ROW + i);
		}
		else {
			get_menu_lm()->clr_led_x(LED_B_IDX, func_id * MATRIX_NR_ROW + i);
		}
	}	
	return ret;
}
void test_proj_two::update_ui(){
}