#include "fct_step.h"
#include "../errata.h"

#define BASE10	10
#define LONG_PRESS_MS	100

void fct_step::init(sequencer* seq, char* name){
	_seq = seq;
	_lp_cnt = 0;
	fct_clbk::set_fct_name(name);
}

static void color_active_step(track* t, uint8_t btn){
	uint8_t id = errata_btn[btn];
	if(t->arr_step[id].is_step_active()){
		Serial.println("active");
		t->get_led_matrix()->clr_n_restore(btn, BACKGROUND);
	}
	else {
		Serial.println("inactive");
		t->get_led_matrix()->save_n_set(LED_R_IDX, btn, BACKGROUND);
	}
}

static void clear_all_long_pushed_ui(track* t, uint8_t* lp_cnt, struct led_blink_t* lp_ui){
//	track* t = _seq->get_current_track();
	for(uint8_t i=0; i<*lp_cnt; i++){
		t->get_led_matrix()->clr_n_restore(lp_ui[i]._id, FOREGROUND1);
	}
	*lp_cnt = 0;
}

static bool btn_was_long_pushed(uint8_t btn_id, uint8_t lp_cnt, const struct led_blink_t* lp_ui){
	bool flg = false;
	for(uint8_t i=0; i<lp_cnt; i++){
		if(btn_id == lp_ui[i]._id){
			flg = true;
		}
	}
	return flg;
}


void fct_step::on_push(uint8_t btn_id){
	char str[7];
	uint8_t id = errata_btn[btn_id];
	_seq->prog::display_str(itoa(btn_id,str,BASE10), 2);
	track* t = _seq->get_current_track();

	if(!t->arr_step[id].is_step_active()){
		t->get_led_matrix()->toogle_led_x(LED_R_IDX, btn_id);
	}
}
void fct_step::on_release(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();

//	Serial.println("short relase");
	
	// check if the pushed button was long pushed button

	if(_lp_cnt == 1){
		// linked step
		if(!btn_was_long_pushed(btn_id, _lp_cnt, _lp_ui)){
			// linked step together
			uint8_t first_push;
			uint8_t second_push;
			if(errata_btn[_lp_ui[_lp_cnt-1]._id] < errata_btn[btn_id]){
				first_push = errata_btn[_lp_ui[_lp_cnt-1]._id];
				second_push = errata_btn[btn_id];
			} else {
				second_push = errata_btn[_lp_ui[_lp_cnt-1]._id];
				first_push = errata_btn[btn_id];
			}

//			for(uint8_t i=first_push+1;i<=second_push;i++){
//				t->arr_step[i].set_step_active();
//				t->arr_step[i].link_step();
//				t->get_led_matrix()->save_n_set(LED_R_IDX, errata_step[i], BACKGROUND);	
//			}
//			t->arr_step[second_push].set_step_active();
//			t->get_led_matrix()->save_n_set(LED_R_IDX, errata_step[second_push], BACKGROUND);

			for(uint8_t i=first_push;i<second_push;i++){
				t->arr_step[i].set_step_active();
				t->arr_step[i].link_step();
				t->get_led_matrix()->save_n_set(LED_R_IDX, errata_step[i], BACKGROUND);	
			}
			t->arr_step[second_push].set_step_active();
			t->get_led_matrix()->save_n_set(LED_R_IDX, errata_step[second_push], BACKGROUND);
		}
		clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);
	} 
	else if(_lp_cnt == 2){
		// create sub-track

	} 
	else {
		if(t->arr_step[id].is_step_active()){
			// check if the step was linked
			if(t->arr_step[id].is_step_linked()){
				// TODO should check track len and negative index
				t->arr_step[id-1].unlink_step();
				t->arr_step[id].unlink_step();
			}
//			 else {
				t->arr_step[id].clr_step_active();
				t->get_led_matrix()->clr_n_restore(btn_id, BACKGROUND);
//			}
		} else { 
			t->arr_step[id].set_step_active();
			t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, BACKGROUND);
		}
		
	}
}
void fct_step::on_long_push(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();
	
	if(_lp_cnt < BTN_MAX_LONG_PUSH_STATE){
		_lp_ui[_lp_cnt]._ms = 0;
		_lp_ui[_lp_cnt]._id = btn_id;
		t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, FOREGROUND1);
		++_lp_cnt;
	}
	else {
//		_lp_cnt = 0;
		t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, FOREGROUND1);
//		for(uint8_t i=0; i<BTN_MAX_LONG_PUSH_STATE; i++){
//			t->get_led_matrix()->clr_n_restore(_lp_ui[i]._id, FOREGROUND1);
//		}
		clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui); 
		t->get_led_matrix()->clr_n_restore(btn_id, FOREGROUND1);
	}
	
}
void fct_step::on_long_release(uint8_t btn_id){
	Serial.println("on long release");
}

void fct_step::update_ui(uint32_t mst_ms, uint16_t mst_step){
	track* t = _seq->get_current_track();
	for(uint8_t i = 0; i<_lp_cnt; i++){
		if(_lp_ui[i]._ms >= LONG_PRESS_MS){
			t->get_led_matrix()->toogle_led_x(LED_R_IDX, _lp_ui[i]._id);
			_lp_ui[i]._ms = 0;
		}
	}
}

void fct_step::on_start(){
}
void fct_step::on_leave(){
	clear_all_long_pushed_ui(_seq->get_current_track(), &_lp_cnt, _lp_ui);
//	track* t = _seq->get_current_track();
//	for(uint8_t i=0; i<_lp_cnt; i++){
//		t->get_led_matrix()->clr_n_restore(_lp_ui[i]._id, FOREGROUND1);
//	}
//	_lp_cnt = 0;
}
