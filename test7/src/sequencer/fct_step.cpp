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

		if(!t->_mtx_btn_to_step[prev_step]->is_step_linked()){
			t->_mtx_btn_to_step[step_cnt]->set_step_up();
			t->_mtx_btn_to_step[step_cnt]->link_step();

			t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[step_cnt]->get_step_color(), errata_step[step_cnt], BACKGROUND);
			res = true;
			step_cnt = (step_cnt + 1) % t->get_max_step();
		}	

	while(step_cnt != second){
		t->_mtx_btn_to_step[step_cnt]->set_step_off();
		t->_mtx_btn_to_step[step_cnt]->link_step();
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[step_cnt]->get_step_color(), errata_step[step_cnt], BACKGROUND);
		step_cnt = (step_cnt + 1) % t->get_max_step();
	}
	
	// check if step after "second" is linked 
	if(!t->_mtx_btn_to_step[(step_cnt + 1) % t->get_max_step()]->is_step_linked()){
		t->_mtx_btn_to_step[step_cnt]->set_step_dw();
		t->_mtx_btn_to_step[step_cnt]->link_step();
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[step_cnt]->get_step_color(), errata_step[step_cnt], BACKGROUND);
	}
	else {
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
}


static void shift_step_ui(track* t, step* s, int shift){
	int ret;
	uint8_t id = s->_step_ui_id + shift;
	Serial.print("shift_step_ui ");
	Serial.println(id);	

	if( id >= NR_STEP ){
		id = NR_STEP - 1;
		t->_mtx_btn_to_step[id] = NULL;
	}
	else {
//		t->get_led_matrix()->clr_n_restore(errata_step[id], BACKGROUND);	
		t->_mtx_btn_to_step[id] = s;
		if(s->is_step_active()){
			Serial.print("active step ");
			Serial.println(errata_step[id]);
			ret = t->get_led_matrix()->save_n_set(s->get_step_color(), errata_step[id], BACKGROUND);
			if(ret < 0)
				Serial.println("Fail to save_n_set");
		}
	}
	s->_step_ui_id = id;
	
}

void fct_step::on_push(uint8_t btn_id){
	char str[7];
	uint8_t id = errata_btn[btn_id];
	_seq->prog::display_str(itoa(btn_id,str,BASE10), 2);
	track* t = _seq->get_current_track();

	if(t->_mtx_btn_to_step[id]){
		if(!t->_mtx_btn_to_step[id]->is_step_active()){
			t->get_led_matrix()->toogle_led_x(t->_mtx_btn_to_step[id]->get_step_color(), btn_id);
		}
	} 
	// else do nothing
}
void fct_step::on_release(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();

	// check if the pushed button was long pushed button
	if(_lp_cnt == 1){
		// linked step
		/* Temporarily disabled */
	   /*	
		if(!btn_was_long_pushed(btn_id, _lp_cnt, _lp_ui)){
			if(link_steps(_seq, errata_btn[_lp_ui[_lp_cnt-1]._id], errata_btn[btn_id])){
				_seq->_ls_ui.ms_cnt = 0;
				_seq->_ls_ui.step_id = _lp_ui[_lp_cnt-1]._id;
			}

		}
		*/
		clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);
	} 
	else if(_lp_cnt == 2){
		uint8_t from = errata_btn[_lp_ui[0]._id];
		uint8_t to = errata_btn[_lp_ui[1]._id];
		uint8_t nr_new_step = id + 1;
		
		// clear ui
		clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);
		t->get_led_matrix()->toogle_led_x(t->_mtx_btn_to_step[id]->get_step_color(), btn_id);
	
		if(from > to){
			Serial.println("unable to insert sub-track");
		} 
		else {
			Serial.print("from ");
			Serial.print(from);
			Serial.print(" to ");
			Serial.print(to);
			Serial.print(" nr_new_step ");
			Serial.println(nr_new_step);
			// clear step from:to-1
/*
			for(int i=from;i<(to);i++){
				t->_mtx_btn_to_step[i]->clr_step_active();
				t->get_led_matrix()->clr_n_restore(errata_step[i], BACKGROUND);	
			}

			// shift ui
			step* s = t->_mtx_btn_to_step[to];
			step* step_to = s;
			for(int i=to;i<NR_STEP;i++){
				t->get_led_matrix()->clr_n_restore(errata_step[i], BACKGROUND);
			}
			while(s != &t->arr_step[0]){
				shift_step_ui(t, s, (nr_new_step-(to-from)));
				s = s->get_next_step();
			}

			// create new track
			track* st = new track(nr_new_step-1);
//		       	st->get_clk()->clk_set_ratio(t->get_clk()->clk_get_ms(),(to-from), nr_new_step);
			
			// chain new steps
			t->_mtx_btn_to_step[from]->set_next_step(&(st->arr_step[0]));
			st->arr_step[nr_new_step-2].set_next_step(step_to);

			t->_mtx_btn_to_step[from]->_clk_def.numerator = (to-from);
			t->_mtx_btn_to_step[from]->_clk_def.denominator = nr_new_step;

			// set ui id
			for(int i=1;i<(nr_new_step);i++){
				t->_mtx_btn_to_step[from+i] = &(st->arr_step[i-1]);
				t->_mtx_btn_to_step[from+i]->_step_ui_id = from + i;
				t->_mtx_btn_to_step[from+i]->_clk_def.numerator = (to-from);
				t->_mtx_btn_to_step[from+i]->_clk_def.denominator = nr_new_step;
			}
*/			

		}

	} 
	else {
		if(t->_mtx_btn_to_step[id]){
			if(t->_mtx_btn_to_step[id]->is_step_active()){
//				// TODO should check track len and negative index
				/* temporarily disabled */
				/*
				if(t->_mtx_btn_to_step[id]->is_step_linked()){
					unlink_steps(_seq, id);
				}
				*/
				t->_mtx_btn_to_step[id]->clr_step_active();
				t->get_led_matrix()->clr_n_restore(btn_id, BACKGROUND);
			} else { 
				t->_mtx_btn_to_step[id]->set_step_active();
				t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[id]->get_step_color(), btn_id, BACKGROUND);
			}
		} // else do-nothing
	}
}
void fct_step::on_long_push(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();
	
	if(_lp_cnt < BTN_MAX_LONG_PUSH_STATE){
		_lp_ui[_lp_cnt]._ms = 0;
		_lp_ui[_lp_cnt]._id = btn_id;
		t->get_led_matrix()->save_n_set(t->_mtx_btn_to_step[id]->get_step_color(), btn_id, FOREGROUND1);
		++_lp_cnt;
	}
	else {
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
