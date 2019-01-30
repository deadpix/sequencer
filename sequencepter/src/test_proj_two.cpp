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
void test_proj_two::menu_update(uint32_t mst_ms, uint16_t mst_step){
}

void test_proj_two::on_push(uint8_t btn_id){
	UNUSED(btn_id);
}
void test_proj_two::on_release(uint8_t btn_id){
	_lm.save_n_toogle(LED_B_IDX, btn_id, BACKGROUND);
}

int test_proj_two::menu_on_push(uint8_t func_id, uint8_t opt_id){	
	UNUSED(func_id);
	UNUSED(opt_id);
	int ret = 1;
	prog::display_title();
	return ret;
}
int test_proj_two::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	for(int i=0; i<MATRIX_NR_COL; i++){
		if(i == opt_id){
			get_menu_lm()->save_n_set(LED_B_IDX, func_id * MATRIX_NR_ROW + i, BACKGROUND);
		}
		else {
			get_menu_lm()->clr_n_restore(func_id * MATRIX_NR_ROW + i, BACKGROUND);
		}
	}	
	return ret;
}
void test_proj_two::update_ui(uint32_t mst_ms, uint16_t mst_step){
	UNUSED(mst_ms);
	UNUSED(mst_step);
}
