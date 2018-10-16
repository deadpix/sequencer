#include "fct_step.h"
#include "../errata.h"
#include <hw_debug.h>

#define BASE10	10
#define LONG_PRESS_MS	100

void fct_step::init(sequencer* seq, char* name){
	_seq = seq;
	_lp_cnt = 0;
	fct_clbk::set_fct_name(name);
}

static void clear_all_long_pushed_ui(track* t, uint8_t* lp_cnt, struct led_blink_t* lp_ui){
//	track* t = _seq->get_current_track();
	int ret = 0;
	for(uint8_t i=0; i<*lp_cnt; i++){
		ret = t->get_led_matrix()->clr_n_restore(lp_ui[i]._id, FOREGROUND1);
//		dbg::printf("clear %d ret=%d \n",lp_ui[i]._id,ret);
	}
	*lp_cnt = 0;

}

void fct_step::on_push(uint8_t btn_id){
//	char str[7];
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();
	node* n = t->get_node_from_matrix(id);

//	_seq->prog::display_str(itoa(btn_id,str,BASE10), 2);

	if(n){
		// step case
		if(n->_step){
			if(n->_step->is_step_active()){
				t->get_led_matrix()->save_n_toogle(n->_step->get_step_color(), btn_id, FOREGROUND1);
			}
		}
		// node case 
		else {
			dbg::printf("show children nodes\n");
			if(!t->is_playing()){
				t->show_children_node(n);
			}
		}
	}
	// else do nothing
}
void fct_step::on_release(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();
	node* n = t->get_node_from_matrix(id);
	
	t->get_led_matrix()->clr_n_restore(btn_id, FOREGROUND1);
	

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
		// TODO create substep
//		s->_clk_def.numerator = (to-from);
//		s->_clk_def.denominator = nr_new_step;	
		uint8_t len = (id % t->get_max_step()) + 1;
		
		// TODO: first_step might be deleted
//		node* tmp = t->get_first_step()->_node;
		node* start = t->get_node_from_matrix(errata_btn[_lp_ui[0]._id]);
		node* end = t->get_node_from_matrix(errata_btn[_lp_ui[1]._id]);
		node* parent = start->_parent;
			// delete node between start and end
		for(uint8_t i=(start->_node_id+1);i<end->_node_id;i++){
			node* tmp = parent->_children->remove(i);
			t->set_node_in_matrix(tmp->_mtx_id, NULL);
			// should we delete node???
			delete tmp;
		}
		uint8_t new_numerator = start->_step->_clk_def.numerator * (end->_node_id - start->_node_id); 
		uint8_t new_denominator = start->_step->_clk_def.denominator * len; 
		// delete step of start node
		delete start->_step;
		start->_step = NULL;

		// create x nodes and steps
		t->create_tree(start, len, new_numerator, 
			new_denominator, start->_node_lvl * DEFAULT_STEP_PER_SEQ);
		// chain steps
		track::chain_step_from_node_list(start->_children,
				  start->_children->get(0)->_step,
				  start->_children->get(len-1)->_step);

		// FIXME: end might not have a step => should find the first step
		start->_children->get(len-1)->_step->set_next_step(end->get_first_step(NR_STEP/DEFAULT_STEP_PER_SEQ));

		dbg::printf("new steps / nodes have been created\n");
		// create tree
	

		// clear ui
		clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);

		t->get_led_matrix()->clr_n_restore(errata_btn[start->_mtx_id], BACKGROUND);
		t->get_led_matrix()->save_n_set(LED_GBR_IDX, errata_btn[start->_mtx_id], BACKGROUND); 
		t->show_children_node(start);	
	
	} 
	else {			
		if(n && n->_step){
			step* s = t->_mtx_to_node[id]->_step;
			if(s->is_step_active()){
			s->clr_step_active();
				t->get_led_matrix()->clr_n_restore(btn_id, BACKGROUND);
			} else {
				s->set_step_active();
				t->get_led_matrix()->save_n_set(s->get_step_color(), btn_id, BACKGROUND); 
			}
		}
	}
}
void fct_step::on_long_push(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();
	node* n = t->get_node_from_matrix(id);
	int ret;
	
	if(n){	
		// step case
		if(n->_step){
	
			if(_lp_cnt < BTN_MAX_LONG_PUSH_STATE){

				// verify that long push is on the same level as the previous
				bool flg_lvl = true;
				for(int i=0; i<_lp_cnt; i++){
					node *tmp = t->get_node_from_matrix(errata_btn[_lp_ui[i]._id]);
					if( (tmp->_node_lvl != n->_node_lvl) || (tmp == n) ){
						flg_lvl = false;
						break;
					}
				}

				if(flg_lvl){
					_lp_ui[_lp_cnt]._ms = 0;
					_lp_ui[_lp_cnt]._id = btn_id;
					ret = t->get_led_matrix()->save_n_set(n->_step->get_step_color(), btn_id, FOREGROUND1);
		
					dbg::printf("btn_id=%d on long push ret=%x\n",btn_id,ret);
					++_lp_cnt;
				}
				else {
					t->get_led_matrix()->save_n_set(n->_step->get_step_color(), btn_id, FOREGROUND1);
					clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui); 
					t->get_led_matrix()->clr_n_restore(btn_id, FOREGROUND1);
				}
			}
			else {
				t->get_led_matrix()->save_n_set(n->_step->get_step_color(), btn_id, FOREGROUND1);
				clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui); 
				t->get_led_matrix()->clr_n_restore(btn_id, FOREGROUND1);
			}
		}
		// node case
		else {

		}
	} 
	// do nothing
//	else {
//	}
	
}
void fct_step::on_long_release(uint8_t btn_id){
	UNUSED(btn_id);
}

void fct_step::update_ui(uint32_t mst_ms, uint16_t mst_step){
	UNUSED(mst_ms);
	UNUSED(mst_step);

	track* t = _seq->get_current_track();
	for(uint8_t i = 0; i<_lp_cnt; i++){
		if(_lp_ui[i]._ms >= LONG_PRESS_MS){
			node* n = t->get_node_from_matrix(_lp_ui[i]._id);
			if(n && n->_step){
				t->get_led_matrix()->save_n_toogle(n->_step->get_step_color(), _lp_ui[i]._id, FOREGROUND1);
			}
			else {
				dbg::printf("node error matrix id%d\n", _lp_ui[i]._id);
			}
//			t->get_led_matrix()->toogle_led_x(t->_mtx_btn_to_step[_lp_ui[i]._id]->get_step_color(), _lp_ui[i]._id);
//			t->get_led_matrix()->save_n_toogle(t->_mtx_btn_to_step[_lp_ui[i]._id]->get_step_color(), _lp_ui[i]._id, BACKGROUND);
//			t->get_led_matrix()->save_n_toogle(t->_mtx_btn_to_step[_lp_ui[i]._id]->get_step_color(), _lp_ui[i]._id, FOREGROUND1);
			_lp_ui[i]._ms = 0;
		}
	}

//	t->show_signature_change(mst_ms);

}

void fct_step::on_start(){
}
void fct_step::on_leave(){
	clear_all_long_pushed_ui(_seq->get_current_track(), &_lp_cnt, _lp_ui);
}
