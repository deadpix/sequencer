#include "global.h"

global::global(){
}

global::~global(){
}


void global::on_push(void* ptr, uint8_t btn_id){
	;
}

void global::on_release(void* ptr, uint8_t btn_id){
	;
}

led_matrix* global::update_menu(uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){
	Serial.print(func_id);
	Serial.print(" ");
	Serial.println(opt_id);
	if(opt_id == 0){
		play = !play;
		menu_matrix->set_pixel_status(func_id * NUM_LED_ROWS + opt_id, play);
	}
	return NULL;
}

led_matrix* global::menu_clbk(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){
	global* myself = static_cast<global *>(this_ptr);
	led_matrix* lm = myself->update_menu(func_id, opt_id, menu_matrix);
	return lm;
}