#include "test_proj_one.h"

test_proj_one::test_proj_one(){
}

test_proj_one::~test_proj_one(){
}

led_matrix* test_proj_one::get_led_matrix(void){
	return &_lm;
}

void test_proj_one::menu_enter(){
}
void test_proj_one::menu_leave(){
}
void test_proj_one::menu_update(){
}


void test_proj_one::on_push(uint8_t btn_id){
}
void test_proj_one::on_release(uint8_t btn_id){
	_lm.toogle_led_x(LED_R_IDX, btn_id);
}

int test_proj_one::menu_on_push(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	return ret;
}
int test_proj_one::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	for(int i=0; i<MATRIX_NR_COL; i++){
		if(i == opt_id){
			get_menu_lm()->set_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
		}
		else {
			get_menu_lm()->clr_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
		}
	}	
	return ret;
}