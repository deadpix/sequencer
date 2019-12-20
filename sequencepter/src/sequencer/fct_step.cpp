#include "fct_step.h"
#include "../errata.h"
#include <hw_debug.h>
//#include "../../interrupts.h"
#include <interrupts.h>

#define BASE10	10
#define LONG_PRESS_MS	100

void fct_step::init(sequencer* seq, char* name){
	_seq = seq;
	_lp_cnt = 0;
	fct_clbk::set_fct_name(name);
	step_color_ = _seq->get_current_track()->get_track_color();
}

static void clear_all_long_pushed_ui(track* t, uint8_t* lp_cnt, struct led_blink_t* lp_ui){
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
		if(n->_node_is_step){
			if(n->_step->is_step_active()){
				t->get_led_matrix()->save_n_toogle(n->_step->get_step_color(), btn_id, FOREGROUND1);
			}
			// clear from lvl + 1 interface
			t->mask_npo_nodes(n->_node_depth);
		}
		// node case 
		else {
			dbg::printf("show children nodes\n");
            if(!t->track_is_playing()){
				t->show_children_node(n);
			}
		}
	}
	// else do nothing
}

static void delete_children_nodes(LinkedList<node *>* children){
	for(int i=0;i<children->size();i++){
		node* tmp = children->remove(i);
		delete tmp;
	}
}

static void clear_mtx_to_node_from_level(track *t, uint8_t level){
	uint8_t start = level * DEFAULT_STEP_PER_SEQ;
	dbg::printf("start at leebel %d level %d\n",start,level);
	for(int i=start; i<NR_STEP; i++){
		t->set_node_in_matrix(i, NULL);
	}  
}

#define ERR_NODE_IS_NOT_A_STEP  1
#define ERR_NODE_IS_LOOP_START  2
#define ERR_NODE_IS_LOOP_END    3

static int step_is_loop_start(step * s, track * t){
    int ret = 0;
    for(uint8_t i=0;i<SEQ_NR_LOOP_SETTING;i++){
        if(t->get_first_step(i) == s){
            ret = 1;
            break;
        }
    }
    return ret;
}
static int step_is_loop_end(step * s, track * t){
    int ret = 0;
    for(uint8_t i=0;i<SEQ_NR_LOOP_SETTING;i++){
        if(t->get_last_step(i) == s){
            ret = 1;
            break;
        }
    }
    return ret;
}
static int sanity_check(track * t, const uint8_t start_id, const uint8_t end_id){
    const node * n = t->get_node_from_matrix(errata_btn[start_id]);
    const node * end_node = t->get_node_from_matrix(errata_btn[end_id]);
    uint8_t idx = start_id;
    int ret = 0;
    // verify thet node start is also a step
    if(!n->_node_is_step){
        ret = ERR_NODE_IS_NOT_A_STEP;
        dbg::printf("node %d depth %d is not a step\n",n->_node_id,n->_node_depth);
    }
    // verify that node start is not last step in the loops
    if(step_is_loop_end(n->_step, t)){
        ret = ERR_NODE_IS_LOOP_END;
        dbg::printf("node %d depth %d is on the end of the loops\n",n->_node_id,n->_node_depth);
    }

    // get next node
    ++idx;
    if(idx == DEFAULT_STEP_PER_SEQ) idx = 0;
    n = t->get_node_from_matrix(errata_btn[idx]);

    // loop until reahing end node
    while(n != end_node){
        // verify that node is also a step
        if(!n->_node_is_step){
            ret = ERR_NODE_IS_NOT_A_STEP;
            dbg::printf("node %d depth %d is not a step\n",n->_node_id,n->_node_depth);
            break;
        }
        // verify that node is not the first step in the loops
        if(step_is_loop_start(n->_step, t)){
            ret = ERR_NODE_IS_LOOP_START;
            dbg::printf("node %d depth %d is on the start of the loops\n",n->_node_id,n->_node_depth);
        }

        // verify that node is not the last step in the loops
        if(step_is_loop_end(n->_step, t)){
            ret = ERR_NODE_IS_LOOP_END;
            dbg::printf("node %d depth %d is on the end of the loops\n",n->_node_id,n->_node_depth);
        }

        // get next node
        ++idx;
        if(idx == DEFAULT_STEP_PER_SEQ) idx = 0;
        n = t->get_node_from_matrix(errata_btn[idx]);
    }

    // verify thet node end is also a step
    if(!n->_node_is_step){
        ret = ERR_NODE_IS_NOT_A_STEP;
        dbg::printf("node %d depth %d is not a step\n",n->_node_id,n->_node_depth);
    }

    // verify that node end is not the first step in the loops
    if(step_is_loop_start(n->_step, t)){
        ret = ERR_NODE_IS_LOOP_START;
        dbg::printf("node %d depth %d is on the start of the loops\n",n->_node_id,n->_node_depth);
    }

    return ret;
}

void fct_step::on_release(uint8_t btn_id){
	uint8_t id = errata_btn[btn_id];
	track* t = _seq->get_current_track();
	node* n = t->get_node_from_matrix(id);
	
	t->get_led_matrix()->clr_n_restore(btn_id, FOREGROUND1);
	

	// check if the pushed button was long pushed button
	if(_lp_cnt == 1){

		// check if we want to delete a subsequence
        if(btn_id == _lp_ui[0]._id && !t->track_is_playing() && !t->get_node_from_matrix(errata_btn[_lp_ui[0]._id])->_node_is_step){



            DISABLE_IRQ();

			node* start = t->get_node_from_matrix(errata_btn[_lp_ui[0]._id]);
			node* p = start->_parent;
			step* tmp_next = (start->_children->get(start->_children->size()-1))->_step->get_next_step();
			step* tmp_prev = (start->_children->get(0))->_step->get_prev_step();


			// delete "start node" children
			delete_children_nodes(start->_children);
			// Fix GUI
			clear_mtx_to_node_from_level(t, start->_node_depth);
			// create _step at "start node"
			start->_node_is_step = true;
			// restore zombies if exists in "start node" parent
			if(start->_zombies){
				// get index of start in parent list
				int start_idx;
				for(int i=0;i<p->_children->size();i++){
					if(p->_children->get(i) == start){
						start_idx = i;
						break;
					}
				}
				
				step* prev = start->_step;
				node* tmp = NULL;
				for(int i=0;i<start->_zombies->size();i++){
					tmp = start->_zombies->remove(i);
					// restore the zombie node in the node list after
					p->_children->add(start_idx+i+1,tmp);
					prev->set_next_step(tmp->_step);
					tmp->_step->set_prev_step(prev);
					prev = tmp->_step;
				}
				tmp->_step->set_next_step(tmp_next);
				tmp_next->set_prev_step(tmp->_step);
			} 
			else {
				start->_step->set_next_step(tmp_next);
				tmp_next->set_prev_step(start->_step);
			}
			
			start->_step->set_prev_step(tmp_prev);
			tmp_prev->set_next_step(start->_step);


			t->mask_npo_nodes(start->_node_depth);
			clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);
//			t->get_led_matrix()->clr_n_restore(errata_btn[start->_mtx_id], BACKGROUND);
			if(start->_step->is_step_active()){
				t->get_led_matrix()->save_n_ovw(start->_step->get_step_color(), errata_btn[start->_mtx_id], BACKGROUND);
			}
			else {
				t->get_led_matrix()->clr_n_restore(errata_btn[start->_mtx_id], BACKGROUND);	
			}


			ENABLE_IRQ();

		}
		else {
			clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);

		}
	} 
	else if(_lp_cnt == 2){
		// FIXME Must be executed when track is paused...
        int ret;

		uint8_t len = (id % t->get_max_step()) + 1;
		
		// TODO: first_step might be deleted
		node* start = t->get_node_from_matrix(errata_btn[_lp_ui[0]._id]);
		node* end = t->get_node_from_matrix(errata_btn[_lp_ui[1]._id]);

        // foreach nodes,   - verify that there are not the first or last step
        //                  - verify that there is no sub-steps

        ret = sanity_check(t, _lp_ui[0]._id, _lp_ui[1]._id);
        if(ret){
            dbg::printf("unable to create subnode: err %d\n",ret);
            // clear ui
            clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);
        }
        else {

            uint8_t loop_start_flg = step_is_loop_start(start->_step, t);
//            uint8_t loop_end_flg = step_is_loop_end(end->_step, t);

            DISABLE_IRQ();

            node* parent = start->_parent;
            uint8_t new_numerator = start->_step->_clk_def.numerator * (end->_node_id - start->_node_id);
            uint8_t new_denominator = start->_step->_clk_def.denominator * len;

            // limit substep to freq less than 20Hz???

            // put nodes between "start node" and "end node" in "start node" zombies list
            if((start->_node_id+1) < end->_node_id){
                LinkedList<node *> *ll = new LinkedList<node *>;

                for(uint8_t i=(start->_node_id+1);i<end->_node_id;i++){
                    node* tmp = parent->_children->remove(i);
                    ll->add(tmp);
                    // must be sure that nodes are visible!!!
                    t->set_node_in_matrix(tmp->_mtx_id, NULL);
                }
                start->_zombies = ll;
            }

            // delete step of start node
            start->_node_is_step = false;

            // create x nodes and steps
            t->create_tree(start, len, new_numerator,
                new_denominator, start->_node_depth * DEFAULT_STEP_PER_SEQ);

            // chain steps
            step* tmp_prev = start->_step->get_prev_step();
            step* tmp_next = start->_children->get(0)->_step;
            tmp_prev->set_next_step(tmp_next);
            tmp_next->set_prev_step(tmp_prev);

            if(loop_start_flg)
                t->set_first_step(tmp_next);

            tmp_next = end->get_first_step(NR_STEP/DEFAULT_STEP_PER_SEQ);
            tmp_prev = start->_children->get(len-1)->_step;
            tmp_prev->set_next_step(tmp_next);
            tmp_next->set_prev_step(tmp_prev);

            ENABLE_IRQ();

            dbg::printf("new steps / nodes have been created: num=%d denom=%d\n",new_numerator,new_denominator);

            // clear ui
            clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui);
            t->get_led_matrix()->save_n_ovw(LED_GBR_IDX, errata_btn[start->_mtx_id], BACKGROUND);
            t->show_children_node(start);
        }

	
	} 
	else {			
		if(n && n->_node_is_step){
			step* s = t->_mtx_to_node[id]->_step;
			if(s->is_step_active()){
				s->clr_step_active();
				t->get_led_matrix()->clr_n_restore(btn_id, BACKGROUND);
			} else {
				s->set_step_active();
				s->set_step_color(step_color_);
				t->get_led_matrix()->save_n_set(step_color_, btn_id, BACKGROUND); 
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
//		if(n->_step){
		if(n->_node_is_step){
	
			if(_lp_cnt < BTN_MAX_LONG_PUSH_STATE){

				// verify that long push is on the same level as the previous
				bool flg_lvl = true;
				for(int i=0; i<_lp_cnt; i++){
					node *tmp = t->get_node_from_matrix(errata_btn[_lp_ui[i]._id]);
					if( (tmp->_node_depth != n->_node_depth) || (tmp == n) ){
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
			if(_lp_cnt > 0){

				clear_all_long_pushed_ui(t, &_lp_cnt, _lp_ui); 
				t->get_led_matrix()->clr_n_restore(btn_id, FOREGROUND1);

			}
			else {
				dbg::print("long pushed node\n");	
				_lp_ui[_lp_cnt]._ms = 0;
				_lp_ui[_lp_cnt]._id = btn_id;
				ret = t->get_led_matrix()->save_n_set(LED_GBR_IDX, btn_id, FOREGROUND1);
				++_lp_cnt;
			

			}
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
//			if(n && n->_step){
			if(n){
				if(n->_node_is_step){
					t->get_led_matrix()->save_n_toogle(n->_step->get_step_color(), _lp_ui[i]._id, FOREGROUND1);
				}
				else {
					t->get_led_matrix()->save_n_toogle(LED_GBR_IDX, _lp_ui[i]._id, FOREGROUND1);
				}
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
	step_color_ = _seq->get_current_track()->get_track_color();

}
void fct_step::on_leave(){
	clear_all_long_pushed_ui(_seq->get_current_track(), &_lp_cnt, _lp_ui);
}
