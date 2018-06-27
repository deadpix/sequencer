#include "tempo.h"
#include <hw_debug.h>

#define DEBUG				1
#define LED_ANIMATION_MS		60

#define BPM_BTN_ID			0
#define TAP_BTN_ID			1
#define START_BTN_ID			2
#define RESET_BTN_ID			3

#define PLAY_BTN_COLOR			LED_B_IDX
#define PAUSE_BTN_COLOR			LED_R_IDX

static void (*on_tempo_change)(uint32_t ms);

tempo::tempo(){	
	_tap_cnt = 0;
	_in_menu_mode = false;
	_play = false;
}

tempo::~tempo(){
}

void tempo::init(void (*fct)(uint32_t), sequencer* seq, bool play){
	_play = play;
	led_matrix* lm = get_menu_lm();
	_tap_animation.init_animation(lm,(prog_id*MATRIX_NR_COL+TAP_BTN_ID), LED_GBR_IDX);
	_clk_animation.init_animation(lm,(prog_id*MATRIX_NR_COL+BPM_BTN_ID), LED_R_IDX);

	on_tempo_change = fct;
	_seq = seq;
	if(_play){	
//		lm->set_led_x(PLAY_BTN_COLOR, START_BTN_ID);
		lm->save_n_set(PLAY_BTN_COLOR, START_BTN_ID, BACKGROUND);
	} else {
//		lm->set_led_x(PAUSE_BTN_COLOR, START_BTN_ID);
		lm->save_n_set(PAUSE_BTN_COLOR, START_BTN_ID, BACKGROUND);
	}
	_seq->set_track_start(_play);
//	lm->set_led_x(LED_R_IDX, RESET_BTN_ID);
	lm->save_n_set(LED_R_IDX, RESET_BTN_ID, BACKGROUND);
	
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
	}
	_ellapsed_tap = 0;

	if(_tap_cnt == (NR_TAP-1)){
		uint32_t avg = 0;
		for(int i=0; i<NR_TAP; i++){
			avg += _tap_timestamp[i];
		}
		avg /= (NR_TAP-1);
		_mst.clk_bpms_to_bpm(avg);
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
		_clk_animation.turn_on_n_save_led();
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
//	_tap_animation.update_animation();
//	_clk_animation.update_animation();
	_tap_animation.end_animation_n_restore();
	_clk_animation.end_animation_n_restore();
}


int tempo::menu_on_push(uint8_t func_id, uint8_t opt_id){
	int ret = 0;
	prog::display_title();
	if(opt_id == TAP_BTN_ID){
		_tap_animation.start_animation(LED_ANIMATION_MS);
	} 
	else if(opt_id == START_BTN_ID){
		_play = !_play;
		_seq->set_track_start(_play);
		if(_play){
			get_menu_lm()->save_n_ovw(PLAY_BTN_COLOR, opt_id, BACKGROUND);
		}
		else {
			get_menu_lm()->save_n_ovw(PAUSE_BTN_COLOR, opt_id, BACKGROUND);
		}
	}
	else if(opt_id == RESET_BTN_ID){
		_seq->reset_all();
		get_menu_lm()->save_n_ovw(LED_G_IDX, opt_id, BACKGROUND);
		
	}
	return ret;
}
int tempo::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 0;
	if(opt_id == TAP_BTN_ID){
		tap();
		_tap_animation.turn_on_n_save_led();
	} 
	else if(opt_id == RESET_BTN_ID){
		get_menu_lm()->save_n_ovw(LED_R_IDX, opt_id, BACKGROUND);	
	}

	return ret;
}

void tempo::update_ui(uint32_t mst_ms, uint16_t mst_step){
}
