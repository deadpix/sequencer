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

void fct_step::on_push(uint8_t btn_id){
	char str[7];
	uint8_t id = errata_btn[btn_id];
	_seq->prog::display_str(itoa(btn_id,str,BASE10), 2);
	track* t = _seq->get_current_track();
//	color_active_step(t, btn_id);	

//	if(t->arr_step[id].is_step_active()){
////		t->arr_step[id].clr_step_active();
//		t->get_led_matrix()->clr_n_restore(btn_id, BACKGROUND);
//	} else { 
	if(!t->arr_step[id].is_step_active()){
//		t->arr_step[id].set_step_active();
//		t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, BACKGROUND);
		t->get_led_matrix()->toogle_led_x(LED_R_IDX, btn_id);
	}
}
void fct_step::on_release(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();

	Serial.println("short relase");
	
	if(t->arr_step[id].is_step_active()){
		t->arr_step[id].clr_step_active();
		t->get_led_matrix()->clr_n_restore(btn_id, BACKGROUND);
	} else { 
		t->arr_step[id].set_step_active();
		t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, BACKGROUND);
	}
}
void fct_step::on_long_push(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
//	uint8_t lp_arr[BTN_MAX_LONG_PUSH_STATE];
	track* t = _seq->get_current_track();
//	_bs.get_long_push_id(lp_arr);
	Serial.println("on long push");
	
	if(_lp_cnt < BTN_MAX_LONG_PUSH_STATE){
//		_bs.set_long_push(id);
		_lp_ui[_lp_cnt]._ms = 0;
		_lp_ui[_lp_cnt]._id = btn_id;
//		t->get_led_matrix()->clr_n_restore(btn_id, BACKGROUND);
		t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, FOREGROUND1);
		// do something
//		Serial.print("save ");
//		Serial.println(_lp_ui[_lp_cnt]._id);
		++_lp_cnt;
	}
	else {
//		uint8_t 	
		_lp_cnt = 0;
		t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, FOREGROUND1);
		for(uint8_t i=0; i<BTN_MAX_LONG_PUSH_STATE; i++){
//			_bs.clr_long_push(lp_arr[i]);
			t->get_led_matrix()->clr_n_restore(_lp_ui[i]._id, FOREGROUND1);
			Serial.print("restore ");
			Serial.println(_lp_ui[i]._id);

//			color_active_step(t, _lp_ui[i]._id);
		}
		t->get_led_matrix()->clr_n_restore(btn_id, FOREGROUND1);
//			Serial.println("end long push");
	}
	
}
void fct_step::on_long_release(uint8_t btn_id){
	Serial.println("on long release");
}

void fct_step::update_ui(uint32_t mst_ms, uint16_t mst_step){
	track* t = _seq->get_current_track();
	for(uint8_t i = 0; i<_lp_cnt; i++){
		if(_lp_ui[i]._ms >= LONG_PRESS_MS){
		//	Serial.print("update led: ");
		//	Serial.println(_lp_ui[i]._id);
//			t->get_led_matrix()->save_n_toogle(LED_R_IDX, _lp_ui[i]._id, BACKGROUND);
			t->get_led_matrix()->toogle_led_x(LED_R_IDX, _lp_ui[i]._id);
			_lp_ui[i]._ms = 0;
		}
	}
}

void fct_step::on_start(){
}
void fct_step::on_leave(){
	track* t = _seq->get_current_track();
	for(uint8_t i=0; i<_lp_cnt; i++){
		t->get_led_matrix()->clr_n_restore(_lp_ui[i]._id, FOREGROUND1);
	}
	_lp_cnt = 0;
}
