#include "tempo.h"

#define DEBUG	1

tempo::tempo(){	
	_tap_cnt = 0;
}

tempo::~tempo(){
}

void tempo::clr_tap(){
	_tap_cnt = 0;	
}

led_matrix* tempo::get_led_matrix(){
	return NULL;
}

void tempo::tap(){
	if(_tap_cnt > 0){
		_tap_timestamp[tap_cnt-1] = _ellapsed_tap;
	}
	_ellapsed_tap = 0;

	if(_tap_cnt == (NR_TAP-1)){
		uint32_t avg = 0;
		for(int i=0; i<NR_TAP; i++){
			avg += _tap_timestamp[i];
		}
		avg /= NR_TAP;
		_mst.clk_set_ms(avg);
#if DEBUG
		Serial.print("new bpm ");
		Serial.println(_mst.clk_get_bpm());
#endif
	}
	_tap_cnt = (_tap_cnt + 1) % NR_TAP;
}



void tempo::on_push(void* this_ptr, uint8_t btn_id){
	tempo* myself = static_cast<tempo *>(this_ptr);
}
void tempo::on_release(void* this_ptr, uint8_t btn_id){
	tempo* myself = static_cast<tempo *>(this_ptr);
}


led_matrix* tempo::menu_on_push(uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	if(opt_id == 0){
		tap();
		// need to start led animation
	}
	return NULL;
}
led_matrix* tempo::menu_on_release(uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	for(int i=0; i<MATRIX_NR_COL; i++){
	}	
	return NULL;
}
led_matrix* tempo::clbk_menu_on_push(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	tempo* myself = static_cast<tempo *>(this_ptr);
	return myself->menu_on_push(func_id, opt_id, menu_matrix);
}
led_matrix* tempo::clbk_menu_on_release(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
	tempo* myself = static_cast<tempo *>(this_ptr);
	return myself->menu_on_release(func_id, opt_id, menu_matrix);
}