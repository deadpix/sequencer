#include "tempo.h"

#define DEBUG				1
#define LED_ANIMATION_MS	60

#define TAP_BTN_ID			1
#define BPM_BTN_ID			0

static void (*on_tempo_change)(uint32_t ms);

tempo::tempo(){	
	_tap_cnt = 0;
	_in_menu_mode = false;
}

tempo::~tempo(){
}

void tempo::init(void (*fct)(uint32_t)){
	_tap_animation.init_animation(get_menu_lm(),(prog_id*MATRIX_NR_COL+TAP_BTN_ID), LED_GBR_IDX);
	_clk_animation.init_animation(get_menu_lm(),(prog_id*MATRIX_NR_COL+BPM_BTN_ID), LED_R_IDX);

	on_tempo_change = fct;
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
		on_tempo_change(avg);
	}
	_tap_cnt = (_tap_cnt + 1) % NR_TAP;
}

void tempo::on_push(uint8_t btn_id){
}
void tempo::on_release(uint8_t btn_id){
}

clk* tempo::get_mst_clk(){
	return &_mst;
}
		

uint32_t tempo::check_mst_clk(){
	uint32_t ret = _mst.clk_elapsed();
	if(ret && _in_menu_mode){
		_clk_animation.turn_on_led();
		_clk_animation.start_animation(LED_ANIMATION_MS);
	}
	return ret;
}

void tempo::menu_enter(){
	_in_menu_mode = true;
}
void tempo::menu_leave(){
	_in_menu_mode = false;
}

void tempo::menu_update(){
//	check_mst_clk();
	_tap_animation.update_animation();
	_clk_animation.update_animation();
}


int tempo::menu_on_push(uint8_t func_id, uint8_t opt_id){
	int ret = 0;
	if(opt_id == TAP_BTN_ID){
		_tap_animation.start_animation(LED_ANIMATION_MS);
	}
	return ret;
}
int tempo::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 0;
	if(opt_id == TAP_BTN_ID){
		tap();
		_tap_animation.turn_on_led();
	}	
	return ret;
}

void tempo::update_ui(){
}