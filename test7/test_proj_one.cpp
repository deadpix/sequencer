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



void test_proj_one::on_push(/*void* this_ptr, */uint8_t btn_id){
//	test_proj_one* myself = static_cast<test_proj_one *>(this_ptr);
}
void test_proj_one::on_release(/*void* this_ptr, */uint8_t btn_id){
//	test_proj_one* myself = static_cast<test_proj_one *>(this_ptr);
	_lm.toogle_led_x(LED_R_IDX, btn_id);
}

int test_proj_one::menu_on_push(uint8_t func_id, uint8_t opt_id/*, led_matrix* menu_matrix*/){
	int ret = 1;
	return ret;
}
int test_proj_one::menu_on_release(uint8_t func_id, uint8_t opt_id/*, led_matrix* menu_matrix*/){	
	int ret = 1;
	for(int i=0; i<MATRIX_NR_COL; i++){
		if(i == opt_id){
//			menu_matrix->set_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
			get_menu_lm()->set_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
		}
		else {
//			menu_matrix->clr_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
			get_menu_lm()->clr_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
		}
	}	
	return ret;
}

//led_matrix* test_proj_one::menu_on_push(uint8_t func_id, uint8_t opt_id/*, led_matrix* menu_matrix*/){	
//	return &_lm;
//}
//led_matrix* test_proj_one::menu_on_release(uint8_t func_id, uint8_t opt_id/*, led_matrix* menu_matrix*/){	
//	led_matrix* next;
//	for(int i=0; i<MATRIX_NR_COL; i++){
//		if(i == opt_id){
////			menu_matrix->set_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
//			get_menu_lm()->set_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
//			next = &_lm;
//		}
//		else {
////			menu_matrix->clr_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
//			get_menu_lm()->clr_led_x(LED_R_IDX, func_id * MATRIX_NR_ROW + i);
//			next = &_lm;
//		}
//	}	
//	return next;
//}


//led_matrix* test_proj_one::clbk_menu_on_push(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
//	test_proj_one* myself = static_cast<test_proj_one *>(this_ptr);
//	return myself->menu_on_push(func_id, opt_id/*, menu_matrix*/);
//}
//led_matrix* test_proj_one::clbk_menu_on_release(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
//	test_proj_one* myself = static_cast<test_proj_one *>(this_ptr);
//	return myself->menu_on_release(func_id, opt_id/*, menu_matrix*/);
//}