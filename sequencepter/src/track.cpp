#include "track.h"
#include "errata.h"
#include <hw_debug.h>
//#include "../interrupts.h"
#include <interrupts.h>

#define CLK_LEN_PER 	(20)

static void (*_hw_fct)(uint16_t, uint8_t, uint8_t);

static void _dummy_fct(uint16_t arg1, uint8_t arg2, uint8_t arg3){
//	Serial.println("track dummy callback function");
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
}

void track::set_clk_def_lock(uint8_t num, uint8_t denom){
	DISABLE_IRQ();
	_clk_def.numerator = num;
	_clk_def.denominator = denom;
	ENABLE_IRQ();
}

bool track::delete_step(LinkedList<step *> *l, step* s){
	bool ret = false;
	for(int i=0;i<l->size();i++){
		if(s == l->get(i)){
			l->remove(i);
			delete s;
			ret = true;
			break;
		}
	}
	return ret;
}

void track::chain_step_from_node_list(LinkedList<node *> *list, step* start, step* end){
	bool flg_chain = false;

	step* tmp = NULL, *prev = NULL;
	for(int i = 0; i<list->size();i++){
//		dbg::printf("step %d\n",list->get(i)->_step->_step_ui_id);
		if(list->get(i)->_node_is_step){
			tmp = list->get(i)->_step;
			if(tmp == end){
				tmp->set_prev_step(prev);
			}
			else {
				prev->set_next_step(tmp);
				tmp->set_prev_step(prev);
			}
			prev = tmp;
		}
	}

}
void track::create_tree(node* parent, uint8_t max_steps, uint8_t num, 
			uint8_t denom, uint8_t mtx_off){
	LinkedList<node *> *ll = new LinkedList<node *>; 

	dbg::printf("max_steps=%d num=%d denom=%d mtx_off=%d\n",max_steps,num,denom,mtx_off);

	node* n;
	step* s, * prev = NULL;

	for(int i=0; i<max_steps; i++){
		n = new node;
		s = new step;
		
		n->_node_depth = parent->_node_depth + 1;
		n->_parent = parent;
		n->_step = s;
		n->_node_id = i;
		s->_node = n;
		s->_clk_def.numerator = num;
		s->_clk_def.denominator = denom;
//		s->upd_step_gate_len(_c.clk_get_ms());	
	
		n->_mtx_id = mtx_off + i;
//		s->_step_ui_id = mtx_off + i;
		_mtx_to_node[mtx_off + i] = n;
		n->_node_is_step = true;

		if(prev){
			prev->set_next_step(s);
			s->set_prev_step(prev);
		}
		ll->add(n);
		prev = s;
	}
	parent->_children = ll;
}


static void set_mtx_to_node(node** arr, node* n, step* s, uint8_t id){
	n->_mtx_id = id;
//	s->_step_ui_id = id;
//	s->set_step_id(id);
 
	arr[id] = n;
}

static void reset_mtx_to_node(node* arr[], node* parent){
	for(int i=0; i<parent->_children->size(); i++){
		node* n = parent->_children->get(i);
		step* s = parent->_children->get(i)->_step;
		set_mtx_to_node(arr, n, s, i);	
	}
}

//void track::set_first_in_loop(step* first, step* new_first, step* last){
void track::set_first_in_loop(step* new_first, uint8_t loop_id){
	step* first = loop_step_[loop_id].first;
	step* last = loop_step_[loop_id].last;

	// restore the previous link of the old first step
	node* parent = first->_node->_parent;
	uint8_t id = (first->_node->_node_id + _max_step - 1) % _max_step; 
	//TODO add assertion on parent
	node* tmp = parent->_children->get(id);
	first->set_prev_step(tmp->get_last_step(NODE_TREE_MAX_DEPTH(_max_step)));
	
	// chain the new first step
	new_first->set_prev_step(last);
	last->set_next_step(new_first);
	loop_step_[loop_id].first = new_first;
}
//void track::set_last_in_loop(step* last, step* new_last, step* first){
void track::set_last_in_loop(step* new_last, uint8_t loop_id){
	step* first = loop_step_[loop_id].first;
	step* last = loop_step_[loop_id].last;

	// restore the next link of the old last step
	node* parent = last->_node->_parent;
	uint8_t id = (last->_node->_node_id + 1) % _max_step; 
	//TODO add assertion on parent
	node* tmp = parent->_children->get(id);
	last->set_next_step(tmp->get_first_step(NODE_TREE_MAX_DEPTH(_max_step)));
	
	// chain the new first step
	new_last->set_next_step(first);
	first->set_prev_step(new_last);
	loop_step_[loop_id].last = new_last;
}

//void track::set_loop(step* first, step* last){
static void set_loop(step* first, step* last){
	first->set_prev_step(last);
	last->set_next_step(first);
}

//static void dump_step(step* first){
//	step* tmp = first;
//	for(int i=0; i<9; i++){
//		dbg::printf("child%d ui_id=%d\n",i,tmp->_step_ui_id);
//		tmp = tmp->get_next_step();
//	}
//}

void track::reset_loop_settings(step * first, step * last){
    dbg::printf("first 0x%x last 0x%x\n",first,last);
    for(int i=0;i<SEQ_NR_LOOP_SETTING;i++){
        loop_step_[i].first = first;
        loop_step_[i].last = last;
    }
    cur_loop_ = 0;
    set_loop(loop_step_[cur_loop_].first, loop_step_[cur_loop_].last);
}

track::track(){
//	curr_step_id = 0;
	_hw_fct = _dummy_fct;
	_max_step = DEFAULT_STEP_PER_SEQ;
	_clk_def.numerator = 1;
	_clk_def.denominator = 1;
	_mst_clk_cnt = 1;
	track_color_ = LED_R_IDX;
    _state = TRACK_PLAY_STATE;

    head = new node;

    create_tree(head, _max_step, 1, 1, 0);
    reset_mtx_to_node(_mtx_to_node, head);

    _cur_step = head->_children->get(0)->_step;
//	_first_step = _cur_step;
//	_last_step = head._children->get(head._children->size() - 1)->_step;
//	set_loop(_first_step, _last_step);
//	chain_step_from_node_list(head._children, _first_step, _last_step);
//    for(int i=0;i<SEQ_NR_LOOP_SETTING;i++){
//		loop_step_[i].first = _cur_step;
//        loop_step_[i].last = head->_children->get(head->_children->size() - 1)->_step;
//	}
    reset_loop_settings(_cur_step, head->_children->get(head->_children->size() - 1)->_step);
//	dump_step(_first_step);
	
}
track::~track(){
    delete head;
	// need to do clean delete of track


//	for(int i=0;i<_step_list.size();i++){
//		step* s = _step_list.remove(i);
//		delete s;
//	}
}

void track::mask_npo_nodes(uint8_t node_lvl){
	// npo: n plus one
	// first level start at one
	// clear the GUI with all nodes starting from npo
	for(uint8_t i=(node_lvl * DEFAULT_STEP_PER_SEQ);i<NR_STEP;i++){
		get_led_matrix()->clr_n_restore(errata_btn[i], BACKGROUND);
	}
}

void track::show_children_node(node* parent){
	if(parent->_children){
		int i;
		for(i=0;i<parent->_children->size();i++){
			node* tmp = parent->_children->get(i);
//			if(tmp->_step){
			if(tmp->_node_is_step){
				get_led_matrix()->clr_n_restore(errata_btn[tmp->_mtx_id], BACKGROUND);
				_mtx_to_node[tmp->_mtx_id] = tmp;
				if(tmp->_step->is_step_active()){
					get_led_matrix()->save_n_set(tmp->_step->get_step_color(), errata_btn[tmp->_mtx_id], BACKGROUND);
				}
			} 
//			else if(!tmp->_step && (tmp->_children->size() > 0)){
			else if(!tmp->_node_is_step && (tmp->_children->size() > 0)){
				get_led_matrix()->save_n_set(LED_GBR_IDX, errata_btn[tmp->_mtx_id], BACKGROUND);
			} else {
				dbg::printf("empty node %d...\n",i);
			}
		}
		for(i;i<DEFAULT_STEP_PER_SEQ;i++){
			uint8_t idx =  errata_btn[parent->_node_depth * DEFAULT_STEP_PER_SEQ + i];
			_mtx_to_node[idx] = NULL;
			get_led_matrix()->clr_n_restore(idx, BACKGROUND);
		}
	} 
	else {
		dbg::printf("parent node: _mtx_id=%d _node_id=%d without children...\n",parent->_mtx_id,parent->_node_id);
	}
}

void track::show_parent_nodes(node* child, node* parent){
	node* tmp = child->_parent;
	
	// redraw is not needed if the case of direct parent
	if(tmp != parent){	
		dbg::printf("parent level %d child->_node_depth=%d\n",parent->_node_depth,tmp->_node_depth);
		for(int i=0; i<(tmp->_node_depth - parent->_node_depth); i++){
			show_children_node(tmp);
			tmp = tmp->_parent;
		}
	}

}



void track::init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t)){
	_hw_fct = fct;
}

led_matrix* track::get_led_matrix(){
	return &_lm;
}

//uint8_t track::get_track_id(){
//	return _track_id;
//}
//void track::set_track_id(uint8_t id){
//	_track_id = id;
//}

uint8_t track::get_out_id(){
	return _out_id;
}
void track::set_out_id(uint8_t id){
	_out_id = id;
}
void track::set_all_step_note(uint16_t note){
//	step* tmp = _first_step;
	step* tmp = loop_step_[cur_loop_].first;

//	while(tmp != _last_step){
	while(tmp != loop_step_[cur_loop_].last){
		tmp->step_set_note(127, note);
		tmp = tmp->get_next_step();
	}
	tmp->step_set_note(127, note);
	tmp = tmp->get_next_step();

//	for(int i=0;i<_step_list.size();i++){
//		_step_list.get(i)->step_set_note(127, note);
//	}
}
//void track::set_step_note(uint16_t note, uint8_t step_id){
//	if(step_id <= NR_STEP){
//		arr_step[step_id].step_set_note(127, note);		
//	} 
//	else {
//		Serial.println("unable to set note: unknown step_id");
//	}
//}

node* track::get_node_from_matrix(uint8_t id){
	return _mtx_to_node[id];
}
void  track::set_node_in_matrix(uint8_t id, node* n){
	_mtx_to_node[id] = n;
}

void track::rebuild_matrix_nodes(node* parent){
    for(int i=0;i<parent->_children->size();i++){
        node * n = parent->_children->get(i);
        _mtx_to_node[n->_mtx_id] = n;
    }
}


uint8_t track::get_max_step(){
	return _max_step;
}
void track::set_max_step(uint8_t max){
	_max_step = max;
	//TODO


//	step* end;
//	if( (end = _mtx_btn_to_step[max-1]) ){
//		_last_step = end;
//		chain_step(&_step_list, _first_step, _last_step);
//	}



/*	
	if(_max_step < max){
		for(int i=_max_step;i<max;i++){
			arr_step[i-1].set_next_step(&arr_step[i%NR_STEP]);
//			_mtx_btn_to_step[i-1]->set_next_step(_mtx_btn_to_step[i%NR_STEP]);
		}
	} else {
		arr_step[_max_step-1].set_next_step(&arr_step[_max_step%NR_STEP]);
//		_mtx_btn_to_step[_max_step-1]->set_next_step(_mtx_btn_to_step[_max_step%NR_STEP]);
	}
		
	for(int i=max;i<NR_STEP;i++){
		arr_step[i-1].set_next_step(&arr_step[0]);
//		_mtx_btn_to_step[i-1]->set_next_step(_mtx_btn_to_step[0]);
	}
	_max_step = max;
*/
}

void track::show_current_step_nodes_no_irq(){
	DISABLE_IRQ();
	node* common_parent = node::get_common_parent(_cur_step->_node, _prev_step->_node);
	show_parent_nodes(_cur_step->_node, common_parent);
	ENABLE_IRQ();
}

bool track::next_step(uint32_t mst_ms){
	_prev_step = _cur_step;
	_cur_step = _cur_step->get_next_step();

//	dbg::printf("mst_ms=%d _cur_step->_clk_def.numerator=%d _cur_step->_clk_def.denominator=%d step clk %d\n",mst_ms,_cur_step->_clk_def.numerator,_cur_step->_clk_def.denominator,_c.clk_get_ms());

//	Serial.print("denominator ");
//	Serial.println(_clk_def.denominator);

	uint32_t step_ms = _c.clk_set_ratio(mst_ms
	, _clk_def.numerator * _cur_step->_clk_def.numerator
	, _clk_def.denominator * _cur_step->_clk_def.denominator );

	_cur_step->upd_step_gate_len(step_ms);


	// FIXME should not be here	
//	node* common_parent = node::get_common_parent(_cur_step->_node, prev->_node); 
//	show_parent_nodes(_cur_step->_node, common_parent);

	return ( _cur_step->is_step_active() );
}
//uint8_t track::get_current_step(){
//	return curr_step_id;
//}
void track::step_reset(){
	DISABLE_IRQ();
//	_cur_step = _first_step;
	_cur_step = loop_step_[cur_loop_].first;
	ENABLE_IRQ();
}

void track::set_current_loop(uint8_t loop_id){
}


clk* track::get_clk(){
	return &_c;
}


//void track::toogle_step_x(uint8_t id){
//	arr_step[id].toogle_step();
//}

//boolean track::is_step_on(uint8_t id){
//	return arr_step[id].step_status();
//}


//void track::mute(){
//	mute_flg = true;
//}
//void track::unmute(){
//	mute_flg = false;
//}
//void track::toogle_mute(){
//	mute_flg = !mute_flg;
//}
//void track::toogle_play(){
//	_play = !_play;
//}

/*
void track::set_play(bool play){
    _play = play;
}
bool track::is_playing(){
	return _play;
}
*/
void track::_init_animate_parents(step* cur){
	node* tmp = cur->_node;
	uint8_t color = 0;
	if(!cur->is_step_active()){
		color = get_track_color();
	}
    while(tmp != head){
		_step_animation[(tmp->_node_depth-1)].init_clk_animation(&_lm, errata_step[tmp->_mtx_id], color);
		_step_animation[(tmp->_node_depth-1)].start_animation((_c.clk_get_ms_lock() * CLK_LEN_PER / 100.));
		tmp = tmp->_parent;
	}
}
void track::_upd_animate_parents(step *cur){
	node* tmp = cur->_node;
    while(tmp != head){
		_step_animation[(tmp->_node_depth-1)].end_animation_n_restore();
		tmp = tmp->_parent;
	}
}

void track::init_animate_parents_no_irq(){
	step* tmp;
	DISABLE_IRQ();
	tmp = _cur_step;
	ENABLE_IRQ();

	_init_animate_parents(tmp);
	
}
void track::upd_animate_parents_no_irq(){
	step* tmp;
	DISABLE_IRQ();
	tmp = _cur_step;
	ENABLE_IRQ();

	_upd_animate_parents(tmp);
}

void track::stop_step_animation(){
    get_led_matrix()->reset();
}

//static uint32_t trig_cnt = 0;
uint8_t track::check_event(uint32_t ms, uint16_t mst_step_cnt){
	UNUSED(mst_step_cnt);
	uint32_t res = _c.master_sync_ratio(ms, &_mst_clk_cnt);	
	
	if(res){
		bool is_active;
        if(track_is_playing()){
			if((is_active = next_step(ms))){
				if(_cur_step->reset_gate()){
					_hw_fct(_cur_step->_note.pitch, _cur_step->_note.velocity, _out_id);
				}
			}
		}
		return 1;

	} else {
		if(_cur_step->upd_gate()){
			_hw_fct(_cur_step->_note.pitch, 0, _out_id);
		}
		return 0;
	}
}

step* track::get_step_by_btn_id(uint16_t btn_id){
    step* s = NULL;
    if(_mtx_to_node[btn_id] && _mtx_to_node[btn_id]->_node_is_step)
        s = _mtx_to_node[btn_id]->_step;
    return s;
}
node* track::get_node_by_btn_id(uint16_t btn_id){
    node* n = NULL;
    if(_mtx_to_node[btn_id])
        n = _mtx_to_node[btn_id];
    return n;
}
