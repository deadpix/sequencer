#include "track.h"

#define CLK_LEN_PER 	(10)

static void (*_hw_fct)(uint16_t, uint8_t, uint8_t);

static void _dummy_fct(uint16_t arg1, uint8_t arg2, uint8_t arg3){
	Serial.println("track dummy callback function");
}

track::track(){
	curr_step_id = 0;
	track_len = 16;
	elapsed_ms = 0;
	_hw_fct = _dummy_fct;
	
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

boolean track::next_step(){
	curr_step_id = (curr_step_id + 1) % NR_STEP;
	// elapsed_ms = 0;
	return arr_step[curr_step_id].is_step_active();
}

// boolean track::is_curr_step_active(uint32_t ms){
	// uint32_t limit = (arr_step[curr_step_id].get_step_gate_len() * ms) / 100; 
	// if( (elapsed_ms > limit) || !(is_step_on(curr_step_id)) )
		// return false;
	// else
		// return true;
// }

uint8_t track::get_current_step(){
	return curr_step_id;
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

uint32_t track::check_event(uint32_t ms, uint16_t mst_step_cnt/*boolean master_clk_flg, clk* master_clk*/){
	uint32_t res = _c.master_sync(ms, mst_step_cnt);
	
	if(res){
		if(next_step()){
			step s = arr_step[curr_step_id];
			s.reset_gate();
			_hw_fct(s._note.pitch, s._note.velocity, _out_id);
		}
		_step_animation.init_animation(&_lm, curr_step_id, LED_GBR_IDX);
		_step_animation.start_animation((_c.clk_get_ms() * CLK_LEN_PER / 100));
	} else {
		arr_step[curr_step_id].upd_gate();
		if(_step_animation.update_animation()){
			if(arr_step[curr_step_id].is_step_active())
				_lm.set_led_x(LED_R_IDX, curr_step_id);
		}
			
	}
	return res;
	
	// boolean evt_flg = clk.clock_update(master_clk_flg, master_clk);
	// boolean step_state;
	// if(evt_flg){
		// step_state = next_step();
		// _lm.set_pixel(curr_step_id);
	// } else {
		// step_state = is_curr_step_active(clk.getInterval());
	// }

	// if(!mute_flg) hw_dWrite(output_id, step_state);
}

void track::on_push(uint8_t btn_id){
}
void track::on_release(uint8_t btn_id){
	_lm.toogle_led_x(LED_R_IDX, btn_id);
}

//led_matrix* track::update_menu(uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
//	for(int i=0; i<NUM_LED_COLUMNS; i++){
//		if(i == opt_id){
//			menu_matrix->set_pixel(func_id * NUM_LED_ROWS + i);		
//		}
//		else {
//			menu_matrix->clr_pixel(func_id * NUM_LED_ROWS + i);
//		}
//	}
//	return &_lm;
//}
//led_matrix* track::menu_clbk(void * this_ptr, uint8_t func_id, uint8_t opt_id, led_matrix* menu_matrix){	
//	track* myself = static_cast<track *>(this_ptr);
//	led_matrix* lm = myself->update_menu(func_id, opt_id, menu_matrix);
//
//	return lm;
//}

void track::update_ui(){
//	clk.clock_upd_gui(curr_step_id, get_led_matrix(), is_step_on(curr_step_id));
}

//menu_clbk_def track::get_menu_clbk(){
//	return menu_clbk;
//}