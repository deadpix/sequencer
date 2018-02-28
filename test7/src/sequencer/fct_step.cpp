#include "fct_step.h"
#include "../errata.h"

#define BASE10	10
#define LONG_PRESS_MS	100

void fct_step::init(sequencer* seq, char* name){
	_seq = seq;
	_lp_cnt = 0;
	fct_clbk::set_fct_name(name);
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

static bool link_steps(sequencer *s, uint8_t first, uint8_t second){
	track* t = s->get_current_track();
	uint8_t step_cnt = first;
	bool res = false;

	// check first if previous step already linked
	uint8_t prev_step;
	if(step_cnt == 0){
		prev_step = t->get_max_step();
	} else {
		prev_step = step_cnt - 1;
	}
//	if(!t->arr_step[prev_step].is_step_linked()){
	if(!t->_mtx_btn_to_step[prev_step]->is_step_linked()){
//		t->arr_step[step_cnt].set_step_up();
//		t->arr_step[step_cnt].link_step();
		t->_mtx_btn_to_step[step_cnt]->set_step_up();
		t->_mtx_btn_to_step[step_cnt]->link_step();

//		t->get_led_matrix()->save_n_set(LED_R_IDX, errata_step[step_cnt], BACKGROUND);
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[step_cnt]->get_step_color(), errata_step[step_cnt], BACKGROUND);
		res = true;
		step_cnt = (step_cnt + 1) % t->get_max_step();
	}	

	while(step_cnt != second){
//		t->arr_step[step_cnt].set_step_off();
//		t->arr_step[step_cnt].link_step();
//		t->get_led_matrix()->save_n_set(LED_R_IDX, errata_step[step_cnt], BACKGROUND);
		t->_mtx_btn_to_step[step_cnt]->set_step_off();
		t->_mtx_btn_to_step[step_cnt]->link_step();
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[step_cnt]->get_step_color(), errata_step[step_cnt], BACKGROUND);
		step_cnt = (step_cnt + 1) % t->get_max_step();
	}
	
	// check if step after "second" is linked 
	if(!t->_mtx_btn_to_step[(step_cnt + 1) % t->get_max_step()]->is_step_linked()){
//	if(!t->arr_step[(step_cnt + 1) % t->get_max_step()].is_step_linked()){
//		t->arr_step[step_cnt].set_step_dw();
//		t->arr_step[step_cnt].link_step();
//		t->get_led_matrix()->save_n_set(LED_R_IDX, errata_step[step_cnt], BACKGROUND);
		t->_mtx_btn_to_step[step_cnt]->set_step_dw();
		t->_mtx_btn_to_step[step_cnt]->link_step();
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[step_cnt]->get_step_color(), errata_step[step_cnt], BACKGROUND);
	}
	else {
//		t->arr_step[step_cnt].set_step_off();
//		t->arr_step[step_cnt].link_step();
//		t->get_led_matrix()->save_n_set(LED_R_IDX, errata_step[step_cnt], BACKGROUND);
		t->_mtx_btn_to_step[step_cnt]->set_step_off();
		t->_mtx_btn_to_step[step_cnt]->link_step();
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[step_cnt]->get_step_color(), errata_step[step_cnt], BACKGROUND);
	}
	return res;
}

static void unlink_steps(sequencer *s, uint8_t step){
	track* t = s->get_current_track();
	uint8_t tmp;
	
	//TODO reevaluate linked step

	if(step == 0){
		tmp = t->get_max_step();	
	} else {
		tmp = step - 1;
	}


	if(t->_mtx_btn_to_step[tmp]->is_step_active()){
		t->_mtx_btn_to_step[tmp]->set_step_dw();
	}
	tmp = (step + 1) % t->get_max_step();
	if(!t->_mtx_btn_to_step[tmp]->is_step_active()){
		t->_mtx_btn_to_step[tmp]->set_step_up();
	}

/*
	if(t->arr_step[tmp].is_step_active()){
		t->arr_step[tmp].set_step_dw();
	}
	tmp = (step + 1) % t->get_max_step();
	if(!t->arr_step[tmp].is_step_active()){
		t->arr_step[tmp].set_step_up();
	}
	*/
}


void fct_step::on_push(uint8_t btn_id){
	char str[7];
	uint8_t id = errata_btn[btn_id];
	_seq->prog::display_str(itoa(btn_id,str,BASE10), 2);
	track* t = _seq->get_current_track();

//	if(!t->arr_step[id].is_step_active()){
	if(!t->_mtx_btn_to_step[id]->is_step_active()){
//		t->get_led_matrix()->toogle_led_x(LED_R_IDX, btn_id);
		t->get_led_matrix()->toogle_led_x(t->_mtx_btn_to_step[id]->get_step_color(), btn_id);
	}
}
void fct_step::on_release(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();

	// check if the pushed button was long pushed button
	if(_lp_cnt == 1){
		// linked step
		if(!btn_was_long_pushed(btn_id, _lp_cnt, _lp_ui)){
			if(link_steps(_seq, errata_btn[_lp_ui[_lp_cnt-1]._id], errata_btn[btn_id])){
				_seq->_ls_ui.ms_cnt = 0;
				_seq->_ls_ui.step_id = _lp_ui[_lp_cnt-1]._id;
			}

		}
		clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);
	} 
	else if(_lp_cnt == 2){
		// create sub-track

	} 
	else {
		if(t->_mtx_btn_to_step[id]->is_step_active()){
//			// TODO should check track len and negative index
			if(t->_mtx_btn_to_step[id]->is_step_linked()){
				unlink_steps(_seq, id);
			}
			t->_mtx_btn_to_step[id]->clr_step_active();
			t->get_led_matrix()->clr_n_restore(btn_id, BACKGROUND);
		} else { 
			t->_mtx_btn_to_step[id]->set_step_active();
			t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[id]->get_step_color(), btn_id, BACKGROUND);
		}
		
	}
}
void fct_step::on_long_push(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();
	
	if(_lp_cnt < BTN_MAX_LONG_PUSH_STATE){
		_lp_ui[_lp_cnt]._ms = 0;
		_lp_ui[_lp_cnt]._id = btn_id;
//		t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, FOREGROUND1);
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[id]->get_step_color(), btn_id, FOREGROUND1);
		++_lp_cnt;
	}
	else {
//		t->get_led_matrix()->save_n_set(LED_R_IDX, btn_id, FOREGROUND1);
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[id]->get_step_color(), btn_id, FOREGROUND1);
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
//			t->get_led_matrix()->toogle_led_x(LED_R_IDX, _lp_ui[i]._id);
			t->get_led_matrix()->toogle_led_x(t->_mtx_btn_to_step[_lp_ui[i]._id]->get_step_color(), _lp_ui[i]._id);
			_lp_ui[i]._ms = 0;
		}
	}
}

void fct_step::on_start(){
}
void fct_step::on_leave(){
	clear_all_long_pushed_ui(_seq->get_current_track(), &_lp_cnt, _lp_ui);
}
