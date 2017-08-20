test_proj_two::test_proj_two(){
	
}

test_proj_two::~test_proj_two(){
	
}

led_matrix* test_proj_two::update_menu(uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	led_matrix* next; 
	for(int i=0; i<NUM_LED_COLUMNS; i++){
		if(i == opt_id){
			menu_matrix->set_led_x(LED_B_IDX, func_id * NUM_LED_ROWS + i);
			next = &_lm;
		}
		else {
			menu_matrix->clr_pixel(func_id * NUM_LED_ROWS + i);
			next = &_lm;
		}
	}	
	return next;
}

void test_proj_two::on_push(void* this_ptr, uint8_t btn_id){
	test_proj_two* myself = static_cast<test_proj_two *>(this_ptr);
}
void test_proj_two::on_release(void* this_ptr, uint8_t btn_id){
	test_proj_two* myself = static_cast<test_proj_two *>(this_ptr);
	_lm.toogle_led_x(LED_B_IDX, btn_id);
}
led_matrix* test_proj_two::menu_clbk(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	test_proj_two* myself = static_cast<test_proj_two *>(this_ptr);
	led_matrix* lm = myself->update_menu(func_id, opt_id, menu_matrix);
	return lm;
}