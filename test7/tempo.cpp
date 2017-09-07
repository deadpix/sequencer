#include "tempo.h"

#define DEBUG				1
#define LED_ANIMATION_MS	60

#define TAP_BTN_ID			1
#define BPM_BTN_ID			0

tempo::tempo(){	
	_tap_cnt = 0;
}

tempo::~tempo(){
}

void tempo::init(){
	_tap_animation.init_animation(get_menu_lm(),(prog_id*MATRIX_NR_COL+TAP_BTN_ID), LED_GBR_IDX);
	_clk_animation.init_animation(get_menu_lm(),(prog_id*MATRIX_NR_COL+BPM_BTN_ID), LED_R_IDX);
}

void tempo::clr_tap(){
	_tap_cnt = 0;	
}

led_matrix* tempo::get_led_matrix(){
	return NULL;
}

void tempo::tap(){
	if(_tap_cnt > 0){
		_tap_timestamp[(_tap_cnt-1)] = _ellapsed_tap;
		Serial.println(_ellapsed_tap);
	}
	_ellapsed_tap = 0;

	if(_tap_cnt == (NR_TAP-1)){
		uint32_t avg = 0;
		for(int i=0; i<NR_TAP; i++){
			avg += _tap_timestamp[i];
		}
		avg /= (NR_TAP-1);
		_mst.clk_bpms_to_bpm(avg);
#if DEBUG
		Serial.print(" new bpm ");
		Serial.println(_mst.clk_get_bpm());

#endif
	}
	_tap_cnt = (_tap_cnt + 1) % NR_TAP;
}



void tempo::on_push(/*void* this_ptr, */uint8_t btn_id){
}
void tempo::on_release(/*void* this_ptr, */uint8_t btn_id){
}

uint32_t tempo::check_mst_clk(){
	uint32_t ret = _mst.clk_elapsed();
	if(ret){
		_clk_animation.turn_on_led();
		_clk_animation.start_animation(LED_ANIMATION_MS);
	}
	return ret;
}

void tempo::menu_enter(){
}
void tempo::menu_leave(){
}

void tempo::menu_update(){
	check_mst_clk();
	_tap_animation.update_animation();
	_clk_animation.update_animation();
}


int tempo::menu_on_push(uint8_t func_id, uint8_t opt_id/*, led_matrix* menu_matrix*/){	
	int ret = 0;
	if(opt_id == TAP_BTN_ID){
		_tap_animation.start_animation(LED_ANIMATION_MS);
		// need to start led animation
	}
	return ret;
}
int tempo::menu_on_release(uint8_t func_id, uint8_t opt_id/*, led_matrix* menu_matrix*/){	
	int ret = 0;
	if(opt_id == TAP_BTN_ID){
		tap();
		_tap_animation.turn_on_led();
	}	
//	for(int i=0; i<MATRIX_NR_COL; i++){
//	}	
	return ret;
}
//led_matrix* tempo::clbk_menu_on_push(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
//	tempo* myself = static_cast<tempo *>(this_ptr);
//	return myself->menu_on_push(func_id, opt_id/*, menu_matrix*/);
//}
//led_matrix* tempo::clbk_menu_on_release(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
//	tempo* myself = static_cast<tempo *>(this_ptr);
//	return myself->menu_on_release(func_id, opt_id/*, menu_matrix*/);
//}