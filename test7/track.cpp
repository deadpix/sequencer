#include "track.h"
#include "src/errata.h"

#define CLK_LEN_PER 	(20)

static void (*_hw_fct)(uint16_t, uint8_t, uint8_t);

static void _dummy_fct(uint16_t arg1, uint8_t arg2, uint8_t arg3){
	Serial.println("track dummy callback function");
}


						  
track::track(){
	curr_step_id = 0;
	track_len = 16;
	elapsed_ms = 0;
	_hw_fct = _dummy_fct;
	_max_step = NR_STEP;
	_play = false;
	
	for(int i=0;i<NR_STEP;i++){
		arr_step[i].set_step_id(i);
	}
}

track::~track(){
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
	for(int i=0;i<NR_STEP;i++){
		arr_step[i].step_set_note(127, note);				
	}
}
void track::set_step_note(uint16_t note, uint8_t step_id){
	if(step_id <= NR_STEP){
		arr_step[step_id].step_set_note(127, note);		
	} 
	else {
		Serial.println("unable to set note: unknown step_id");
	}
}

uint8_t track::get_max_step(){
	return _max_step;
}
void 	track::set_max_step(uint8_t max){
	_max_step = max;
}
boolean track::next_step(){
	curr_step_id = (curr_step_id + 1) % _max_step; 
	// elapsed_ms = 0;
	return arr_step[curr_step_id].is_step_active();
}
uint8_t track::get_current_step(){
	return curr_step_id;
}
void track::step_reset(){
	curr_step_id = 0;
}


clk* track::get_clk(){
	return &_c;
}


void track::toogle_step_x(uint8_t id){
	arr_step[id].toogle_step();
}

boolean track::is_step_on(uint8_t id){
	return arr_step[id].step_status();
}


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
	uint32_t res = _c.master_sync(ms, mst_step_cnt);
	step s = arr_step[curr_step_id];
	
	// need to determine if there is a new clock
	//  --> res and clk_sync 
	//  --> clk_ellasped() 
	
	if(res){
	
		if(_play){	
			if(next_step()){
				s.reset_gate();
				_hw_fct(s._note.pitch, s._note.velocity, _out_id);
			}
		}
		// step animation only for the current track
		_step_animation.init_animation_n_save(&_lm, errata_step[curr_step_id], LED_GBR_IDX);
		_step_animation.start_animation((_c.clk_get_ms() * CLK_LEN_PER / 100.));

	} else {
		if(arr_step[curr_step_id].upd_gate()){
			_hw_fct(s._note.pitch, 0, _out_id);
		}
               _step_animation.end_animation_n_restore();
	}
	return res;
}

void track::on_push(uint8_t btn_id){

//	if(arr_step[errata_btn[btn_id]].is_step_active())
//		arr_step[errata_btn[btn_id]].clr_step_active();
//	else 
//		arr_step[errata_btn[btn_id]].set_step_active();
//
//	_lm.toogle_led_x(LED_R_IDX, btn_id);
}
void track::on_release(uint8_t btn_id){
}

void track::update_ui(uint32_t mst_ms, uint16_t mst_step){
}
