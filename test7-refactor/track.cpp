#include "track.h"
#include "src/errata.h"

#define CLK_LEN_PER 	(20)

static void (*_hw_fct)(uint16_t, uint8_t, uint8_t);

static void _dummy_fct(uint16_t arg1, uint8_t arg2, uint8_t arg3){
	Serial.println("track dummy callback function");
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

static void chain_step(LinkedList<step *> *list, step* start, step* end){
	bool flg_chain = false;

	for(int i = 0; i<list->size();i++){
		if(list->get(i) == start){
			flg_chain = true;
		}
		if(list->get(i) == end){
			flg_chain = false;
		}
		if(flg_chain){
			list->get(i)->set_next_step(list->get(i+1));
		}
		else {
			list->get(i)->set_next_step(start);
		}
	}
}


track::track(){
	curr_step_id = 0;
	track_len = 16;
	elapsed_ms = 0;
	_hw_fct = _dummy_fct;
	_max_step = NR_STEP;
	_play = false;
	_clk_def.numerator = 1;
	_clk_def.denominator = 1;	
	_mst_clk_cnt = 1;


//	arr_step = new step[NR_STEP];	
	for(int i=0;i<NR_STEP;i++){
		step *s = new step;
//		s.set_next_step(&(arr_step[(i+1)%NR_STEP]));
//		s.set_clk(&_c);
		s->_step_ui_id = i;
		_mtx_btn_to_step[i] = s;
		_step_list.add(s);
		s->set_step_id(_step_list.size() - 1);
	}
	_cur_step = _step_list.get(0);
	_first_step = _step_list.get(0);
       	_last_step = _step_list.get(_step_list.size() - 1);	
	chain_step(&_step_list, _first_step, _last_step);
//	_cur_step = &arr_step[0];
}
/*
track::track(uint8_t nr_step){
	curr_step_id = 0;
	track_len = 16;
	elapsed_ms = 0;
	_hw_fct = _dummy_fct;
	_max_step = nr_step;
	_play = false;
	_clk_def.numerator = 1;
	_clk_def.denominator = 1;


//	arr_step = new step[nr_step];	
	for(int i=0;i<nr_step;i++){
//		arr_step[i].set_step_id(i);
//		arr_step[i].set_next_step(&(arr_step[(i+1)%nr_step]));
//		arr_step[i].set_clk(&_c);

//		arr_step[i]._step_ui_id = i;
//		_mtx_btn_to_step[i] = &arr_step[i];
	}
//	for(int i=nr_step;i<NR_STEP;i++){
//		_mtx_btn_to_step[i] = NULL;
//	}
//	_cur_step = &arr_step[0];
}
*/
track::~track(){
	for(int i=0;i<_step_list.size();i++){
		step* s = _step_list.remove(i);
		delete s;
	}
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
//		arr_step[i].step_set_note(127, note);				
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

uint8_t track::get_max_step(){
	return _last_step->_step_ui_id + 1;
}
void track::set_max_step(uint8_t max){
	step* end;
	if( (end = _mtx_btn_to_step[max-1]) ){
		_last_step = end;
		chain_step(&_step_list, _first_step, _last_step);
	}
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
boolean track::next_step(uint32_t mst_ms){
	_cur_step = _cur_step->get_next_step();
	curr_step_id = _cur_step->_step_ui_id;

	Serial.print("numerator ");
	Serial.print(_clk_def.numerator * _cur_step->_clk_def.numerator);
	Serial.print(" denominator ");
	Serial.println(_clk_def.denominator * _cur_step->_clk_def.denominator);

	

	_c.clk_set_ratio(mst_ms
	, _clk_def.numerator * _cur_step->_clk_def.numerator
	, _clk_def.denominator * _cur_step->_clk_def.denominator );
//		_mst_clk_cnt = 0;
	
	// FIXME: calculate of gate len only when clock is 
	//	  changed or at init time
//	_cur_step->upd_step_gate_len(_cur_step->get_clk()->clk_get_ms());
	_cur_step->upd_step_gate_len(_c.clk_get_ms());	

	return ( _cur_step->is_step_active() );
}
uint8_t track::get_current_step(){
	return curr_step_id;
}
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

uint32_t track::check_event(uint32_t ms, uint16_t mst_step_cnt){
//	uint32_t res = _c.master_sync(ms, mst_step_cnt);
	uint32_t res = _c.master_sync_ratio(ms, /*mst_step_cnt*/&_mst_clk_cnt);	
//	if(ms > 0) _mst_clk_cnt++;
//	uint32_t res = _cur_step->get_clk()->master_sync_ratio(ms, mst_step_cnt);	
//	step s = arr_step[curr_step_id];
	
	if(res){
		if(_play){	
			if(next_step(ms)){
				if(_cur_step->reset_gate()){
					_hw_fct(_cur_step->_note.pitch, _cur_step->_note.velocity, _out_id);
				}
			}
		}
		// step animation only for the current track
		_step_animation.init_animation_n_save(&_lm, errata_step[curr_step_id], LED_GBR_IDX);
//		_step_animation.start_animation((_c.clk_get_ms() * CLK_LEN_PER / 100.));
		_step_animation.start_animation(50);

	} else {
//		if(arr_step[curr_step_id].upd_gate()){
		if(_cur_step->upd_gate()){
			_hw_fct(_cur_step->_note.pitch, 0, _out_id);
		}
		_step_animation.end_animation_n_restore();
	}
	return res;
}

void track::on_push(uint8_t btn_id){
}
void track::on_release(uint8_t btn_id){
}

void track::update_ui(uint32_t mst_ms, uint16_t mst_step){
}
