#include "test_proj_one.h"

#define BTN_LED_DELAY_MS	50

static const uint8_t MIDI_DRUM_GM[16] = {37, 36, 42, 82, 40, 38, 46, 44, 48, 47, 45, 43, 49, 55, 51, 53};

static void (*_hw_fct)(uint16_t, uint8_t, uint8_t);

static void _dummy_fct(uint16_t arg1, uint8_t arg2, uint8_t arg3){
//	Serial.println("midi controller dummy callback function");
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
}


test_proj_one::test_proj_one(){
	_hw_fct = _dummy_fct;
}

test_proj_one::~test_proj_one(){
}

void test_proj_one::init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t)){
	_hw_fct = fct;
}


led_matrix* test_proj_one::get_led_matrix(void){
	return &_lm;
}

void test_proj_one::menu_enter(){
}
void test_proj_one::menu_leave(){
}
void test_proj_one::menu_update(uint32_t mst_ms, uint16_t mst_step){
}


void test_proj_one::on_push(uint8_t btn_id){
	_lm.save_n_set(LED_GBR_IDX, btn_id, FOREGROUND2);
	_hw_fct(MIDI_DRUM_GM[btn_id], 127, 1);
}
void test_proj_one::on_release(uint8_t btn_id){
//	_lm.toogle_led_x(LED_GBR_IDX, btn_id);	
	led_toogle* lt = new led_toogle();
	lt->init_animation(&_lm, btn_id, LED_GBR_IDX);
	lt->start_animation(BTN_LED_DELAY_MS);
	_btn_animation_list.add(lt);

	_hw_fct(MIDI_DRUM_GM[btn_id], 0, 1);
}
void test_proj_one::on_long_release(uint8_t btn_id){
//	_lm.toogle_led_x(LED_GBR_IDX, btn_id);	
	led_toogle* lt = new led_toogle();
	lt->init_animation(&_lm, btn_id, LED_GBR_IDX);
	lt->start_animation(BTN_LED_DELAY_MS);
	_btn_animation_list.add(lt);

	_hw_fct(MIDI_DRUM_GM[btn_id], 0, 1);
}

int test_proj_one::menu_on_push(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	UNUSED(func_id);
	UNUSED(opt_id);
	prog::display_title();

	return ret;
}
int test_proj_one::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	for(int i=0; i<MATRIX_NR_COL; i++){
		if(i == opt_id){
			get_menu_lm()->save_n_set(LED_R_IDX, func_id * MATRIX_NR_ROW + i, FOREGROUND2);
		}
		else {
			get_menu_lm()->clr_n_restore(func_id * MATRIX_NR_ROW + i, FOREGROUND2);
		}
	}	
	return ret;
}
void test_proj_one::update_ui(uint32_t mst_ms, uint16_t mst_step){
	int len = _btn_animation_list.size();
	
	UNUSED(mst_ms);
	UNUSED(mst_step);

	for(int i=0;i<len;i++){
		led_toogle* lt = _btn_animation_list.shift();
//		if(lt->update_animation()){
		if(lt->end_animation_n_restore()){
			lt->turn_off_n_restore_led();
//			lt->end_animation_n_restore();
			delete lt;
		} else {
			_btn_animation_list.add(lt);
		}
	}

//	if(_btn_animation.update_animation())
//		_btn_animation.turn_off_led();
		
}
