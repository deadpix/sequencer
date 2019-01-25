#include "track.h"
#include "errata.h"
#include <hw_debug.h>
#include "../interrupts.h"

#define CLK_LEN_PER 	(20)

static void (*_hw_fct)(uint16_t, uint8_t, uint8_t);

static void _dummy_fct(uint16_t arg1, uint8_t arg2, uint8_t arg3){
//	Serial.println("track dummy callback function");
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
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
/*
	for(int i = 0; i<list->size();i++){
		dbg::printf("step %d\n",list->get(i)->_step->_step_ui_id);

		if(list->get(i)->_node_is_step){
			if(list->get(i)->_step == start){
				flg_chain = true;
			}
			if(list->get(i)->_step == end){
				flg_chain = false;
			}
			if(flg_chain){
				list->get(i)->_step->set_next_step(list->get(i+1)->_step);
			
			}
			else {
				list->get(i)->_step->set_next_step(start);
			}
		}
	}
*/
	step* tmp = NULL, *prev = NULL;
	for(int i = 0; i<list->size();i++){
		dbg::printf("step %d\n",list->get(i)->_step->_step_ui_id);
		if(list->get(i)->_node_is_step){
			tmp = list->get(i)->_step;
			if(tmp == start){
//				flg_chain = true;
//				tmp->set_prev_step(end);
			}
			else if(tmp == end){
//				tmp->set_next_step(start);
				tmp->set_prev_step(prev);
			}
			else {
				prev->set_next_step(tmp);
				tmp->set_prev_step(prev);
			}
			prev = tmp;
			

//			if(list->get(i)->_step == end){
//				flg_chain = false;
//			}
//			if(flg_chain){
//				list->get(i)->_step->set_next_step(list->get(i+1)->_step);
//			
//			}
//			else {
//				list->get(i)->_step->set_next_step(start);
//			}
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
		
		n->_node_lvl = parent->_node_lvl + 1;
		n->_parent = parent;
		n->_step = s;
		n->_node_id = i;
		s->_node = n;
		s->_clk_def.numerator = num;
		s->_clk_def.denominator = denom;

		n->_mtx_id = mtx_off + i;
		s->_step_ui_id = mtx_off + i;
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
	s->_step_ui_id = id;
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

void track::set_loop(step* first, step* last){
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

track::track(){
//	curr_step_id = 0;
	_hw_fct = _dummy_fct;
	_max_step = DEFAULT_STEP_PER_SEQ;
	_play = false;
	_clk_def.numerator = 1;
	_clk_def.denominator = 1;
	_mst_clk_cnt = 1;

	create_tree(&head, _max_step, 1, 1, 0);
	reset_mtx_to_node(_mtx_to_node, &head);

	_cur_step = head._children->get(0)->_step;
	_first_step = _cur_step;
	_last_step = head._children->get(head._children->size() - 1)->_step;
//	chain_step_from_node_list(head._children, _first_step, _last_step);
	set_loop(_first_step, _last_step);
//
//	dump_step(_first_step);
	
}
track::~track(){

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
			uint8_t idx =  errata_btn[parent->_node_lvl * DEFAULT_STEP_PER_SEQ + i];
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
		dbg::printf("parent level %d child->_node_lvl=%d\n",parent->_node_lvl,tmp->_node_lvl);
		for(int i=0; i<(tmp->_node_lvl - parent->_node_lvl); i++){
			show_children_node(tmp);
			tmp = tmp->_parent;
		}
	}

}



int track::add_signature_change(step* s, uint8_t num, uint8_t denom, uint8_t color){
	struct signature_change* sc;

	// sanity check
	for(int i = 0; i<_signature_change_list.size();i++){
		step* tmp = _signature_change_list.get(i)->s;
		if(s == tmp){
			return -1;
		}
	}
	
	sc = new signature_change();	
//	new(sc) signature_change;
	sc->s = s;
	sc->num = num;
	sc->denom = denom;
	sc->color = color;
	sc->signature_ui = new led_toogle();
	sc->signature_ui->init_animation(&_lm, s->_step_ui_id, color, FOREGROUND1);
	
	
	_signature_change_list.add(sc);	
	return 0;
	
}

int track::del_signature_change(step* s){
	int error = -1;
	for(int i = 0; i<_signature_change_list.size();i++){
		struct signature_change* sc = _signature_change_list.get(i);
		if(sc->s == s){
			_signature_change_list.remove(i);
			delete sc->signature_ui;
			delete sc;
			error = 0;
			break;
		}
	}
	return error;
}

//static void update_signature_animation(struct signature_change* sc, uint32_t ms){
//	if( sc->signature_ui->end_animation_n_restore() ){
////		led_matrix* lm = sc->signature_ui->get_led_matrix();
////		uint8_t new_color = lm->get_ground_color(sc->s->_step_ui_id, BACKGROUND);
//		
//		sc->signature_ui->start_animation( (ms * sc->num / sc->denom) * 0.2 );
//	}
//}

void track::show_signature_change(uint32_t ms){
	UNUSED(ms);
//	struct signature_change* sc;
//	for(int i = 0; i<_signature_change_list.size();i++){
//		sc = _signature_change_list.get(i);
//		// TODO
//		if(_mtx_btn_to_step[sc->s->_step_ui_id]){
//			update_signature_animation(sc,ms);
//		}
//	}
}

void track::init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t)){
	_hw_fct = fct;
}

led_matrix* track::get_led_matrix(){
	return &_lm;
}

uint8_t track::get_track_id(){
	return _track_id;
}
void track::set_track_id(uint8_t id){
	_track_id = id;
}

uint8_t track::get_out_id(){
	return _out_id;
}
void track::set_out_id(uint8_t id){
	_out_id = id;
}
void track::set_all_step_note(uint16_t note){

	for(int i=0;i<_step_list.size();i++){
		_step_list.get(i)->step_set_note(127, note);
	}
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
bool track::next_step(uint32_t mst_ms){
	step* prev = _cur_step;
	_cur_step = _cur_step->get_next_step();

	dbg::printf("mst_ms=%d _cur_step->_clk_def.numerator=%d _cur_step->_clk_def.denominator=%d step clk %d\n",mst_ms,_cur_step->_clk_def.numerator,_cur_step->_clk_def.denominator,_c.clk_get_ms());
	
	_c.clk_set_ratio(mst_ms
	, _clk_def.numerator * _cur_step->_clk_def.numerator
	, _clk_def.denominator * _cur_step->_clk_def.denominator );


	// FIXME: calculate of gate len only when clock is 
	//	  changed or at init time
//	_cur_step->upd_step_gate_len(_cur_step->get_clk()->clk_get_ms());
	_cur_step->upd_step_gate_len(_c.clk_get_ms());	


	// FIXME should not be here	
//	node* common_parent = node::get_common_parent(_cur_step->_node, prev->_node); 
//	show_parent_nodes(_cur_step->_node, common_parent);

	return ( _cur_step->is_step_active() );
}
//uint8_t track::get_current_step(){
//	return curr_step_id;
//}
void track::step_reset(){
	_cur_step = _first_step;
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


void track::mute(){
	mute_flg = true;
}
void track::unmute(){
	mute_flg = false;
}
void track::toogle_mute(){
	mute_flg = !mute_flg;
}
void track::toogle_play(){
	_play = !_play;
}
void track::set_play(bool play){
	_play = play;
}
bool track::is_playing(){
	return _play;
}

void track::_init_animate_parents(step* cur){
	node* tmp = cur->_node;

	uint8_t color = 0;
	if(!cur->is_step_active()){
		color = LED_R_IDX;
	}
	// step animation only for the current track
	while(tmp != &head){
		_step_animation[(tmp->_node_lvl-1)].init_clk_animation(&_lm, errata_step[tmp->_mtx_id], color);
		_step_animation[(tmp->_node_lvl-1)].start_animation((_c.clk_get_ms() * CLK_LEN_PER / 100.));
		tmp = tmp->_parent;
	}
}
void track::_upd_animate_parents(step *cur){
	node* tmp = cur->_node;
	while(tmp != &head){
		_step_animation[(tmp->_node_lvl-1)].end_animation_n_restore();
		tmp = tmp->_parent;
	}
}

void track::init_animate_parents_no_irq(){
	step* tmp;
	unsigned char reg = disable_irq();
	tmp = _cur_step;
	enable_irq(reg);
	_init_animate_parents(tmp);
	
}
void track::upd_animate_parents_no_irq(){
	step* tmp;
	unsigned char reg = disable_irq();
	tmp = _cur_step;
	enable_irq(reg);
	_upd_animate_parents(tmp);
}

uint32_t track::check_event(uint32_t ms, uint16_t mst_step_cnt){
	UNUSED(mst_step_cnt);
	uint32_t res = _c.master_sync_ratio(ms, &_mst_clk_cnt);	
	
	if(res){
		bool is_active;
		if(_play){	
			if((is_active = next_step(ms))){
				if(_cur_step->reset_gate()){
					_hw_fct(_cur_step->_note.pitch, _cur_step->_note.velocity, _out_id);
				}
			}
		}
		// TODO should not be here!!!
//		_init_animate_parents(_cur_step);

	} else {
		if(_cur_step->upd_gate()){
			_hw_fct(_cur_step->_note.pitch, 0, _out_id);
		}
		// TODO should not be here!!!
//		_upd_animate_parents(_cur_step);
	}
	return res;
}

//void track::on_push(uint8_t btn_id){
//	UNUSED(btn_id);
//}
//void track::on_release(uint8_t btn_id){
//	UNUSED(btn_id);
//}
//
//void track::update_ui(uint32_t mst_ms, uint16_t mst_step){
//	UNUSED(mst_ms);
//	UNUSED(mst_step);
//}
