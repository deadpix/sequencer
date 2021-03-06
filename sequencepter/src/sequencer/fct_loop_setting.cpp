#include "fct_loop_setting.h"
#include "../errata.h"

#include <cstdlib>
#include <hw_debug.h>

#define BASE10	10
#define LED_ANIMATION_MS	50





static void upd_display(sequencer* seq, uint8_t val){
	UNUSED(seq);
	UNUSED(val);
//	char str[7];
//	seq->prog::display_str(itoa(val,str,BASE10), 2);
}

void fct_loop_setting::init(sequencer* seq, char* name){
	_seq = seq;
	fct_clbk::set_fct_name(name);

	track* t = _seq->get_current_track();
	start_loop_ = (t->get_first_step())->_node;
	end_loop_ = (t->get_last_step())->_node;
	
//	loop_animation_[0]->init_animation(t->get_led_matrix(), start_loop_->_mtx_id, LED_GBR_IDX, FOREGROUND1);
//	loop_animation_[1]->init_animation(t->get_led_matrix(), end_loop_->_mtx_id, LED_GBR_IDX, FOREGROUND1);

}

void fct_loop_setting::on_push(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();
	uint8_t nr_step = t->get_max_step();

	if(id < nr_step){

		node* tmp = t->get_node_from_matrix(id);
		if(loop_cnt_ == 0){
			t->set_first_in_loop(tmp->get_first_step(NODE_TREE_MAX_DEPTH(t->get_max_step())), t->get_current_loop_id());
			start_loop_ = tmp;
		} 
		else {
			t->set_last_in_loop(tmp->get_last_step(NODE_TREE_MAX_DEPTH(t->get_max_step())), t->get_current_loop_id()); 
			end_loop_ = tmp;
		}
		
//		t->set_loop(start_loop_->_step, end_loop_->_step);

		// GUI 
		t->get_led_matrix()->clr_n_restore(loop_animation_[loop_cnt_].led_id, FOREGROUND1);
		t->get_led_matrix()->save_n_set(LED_GBR_IDX, id, FOREGROUND1);
		loop_animation_[loop_cnt_].led_id = id;
	
		// button flag
		loop_cnt_ = (loop_cnt_ + 1) % 2;
	}
}
void fct_loop_setting::on_release(uint8_t btn_id){
	UNUSED(btn_id);
}
void fct_loop_setting::on_long_release(uint8_t btn_id){
	UNUSED(btn_id);
}

static void update_loop_animation(struct loop_animation* la, uint32_t deadline, track* t){
	if(la->time_cnt >= deadline){
		if(la->is_turned_on){
			t->get_led_matrix()->clr_n_restore(la->led_id, FOREGROUND1);
		}
		else {
			t->get_led_matrix()->save_n_set(LED_GBR_IDX, la->led_id, FOREGROUND1);
		}
		la->is_turned_on = !la->is_turned_on;
		la->time_cnt = 0;
	}
}

void fct_loop_setting::update_ui(uint32_t mst_ms, uint16_t mst_step){
	UNUSED(mst_ms);
	UNUSED(mst_step);
	track* t = _seq->get_current_track();

	update_loop_animation(&(loop_animation_[0]), LED_ANIMATION_MS, t);
	if(start_loop_ != end_loop_){
		update_loop_animation(&(loop_animation_[1]), LED_ANIMATION_MS, t);
	}

}
void fct_loop_setting::on_start(){
	loop_cnt_ = 0;
	track* t = _seq->get_current_track();
	uint8_t nr_step = t->get_max_step();

	upd_display(_seq, nr_step);

	// get loop nodes
	start_loop_ = (t->get_first_step())->_node;
	end_loop_ = (t->get_last_step())->_node;

	loop_animation_[0].time_cnt = 0;
	loop_animation_[0].led_id = start_loop_->_mtx_id;
	loop_animation_[1].time_cnt = 0;		
	loop_animation_[1].led_id = end_loop_->_mtx_id;
	
	t->get_led_matrix()->save_n_set(LED_GBR_IDX, loop_animation_[0].led_id, FOREGROUND1);
	t->get_led_matrix()->save_n_set(LED_GBR_IDX, loop_animation_[1].led_id, FOREGROUND1);

//	t->get_led_matrix()->save_n_set(LED_GBR_IDX, errata_step[nr_step-1], FOREGROUND1);
//	if(start_loop_ == end_loop_){
//		loop_animation_[0].start_animation(LED_ANIMATION_MS);
//	} 
//	else {
//		loop_animation_[0].start_animation(LED_ANIMATION_MS);
//		loop_animation_[1].start_animation(LED_ANIMATION_MS);	
//	}
	
}
void fct_loop_setting::on_leave(){	
	track* t = _seq->get_current_track();
//	uint8_t nr_step = t->get_max_step();
//	t->get_led_matrix()->clr_n_restore(errata_step[nr_step-1], FOREGROUND1);	
	t->get_led_matrix()->clr_n_restore(start_loop_->_mtx_id, FOREGROUND1);
	t->get_led_matrix()->clr_n_restore(end_loop_->_mtx_id, FOREGROUND1);
}

