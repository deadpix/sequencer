#include "midi_controller.h"
#include "bit.h"
#define BTN_LED_DELAY_MS	50

static void (*_hw_fct)(uint16_t, uint8_t, uint8_t);

static void _dummy_fct(uint16_t arg1, uint8_t arg2, uint8_t arg3){
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
}

midi_controller::midi_controller(){
	_hw_fct = _dummy_fct;
//	midi_out_ = 2;
	for(int i=0;i<MATRIX_NR_ROW;i++){
		for(int j=0;j<KEYBOARD_NR_ROW;j++){
			conf_[i].kb_mask_[j] = 0x0;
		}
		conf_[i].id = i;
		conf_[i].midi_out_ = i;
	}
	current_conf_ = &conf_[0];
	display_midi_keyboard(current_conf_);
}

midi_controller::~midi_controller(){
}

void midi_controller::init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t)){
	_hw_fct = fct;
}


led_matrix* midi_controller::get_led_matrix(void){
	return &lm_;
}

void midi_controller::display_midi_keyboard(struct midi_controller_conf* c){
//	for(int i=0; i<(KEYBOARD_NR_ROW/2); i++){
	for(int i=0; i<4; i=i+2){
		c->kb_mask[i] = 0xff;
		c->kb_mask[i+1] = 0xff;
		c->kb.display_keys(&lm_, i);
		c->kb.display_scale(&lm_, i);
		c->kb.display_root(&lm_, i);
	}
}

void midi_controller::menu_enter(){
}
void midi_controller::menu_leave(){
}
void midi_controller::menu_update(uint32_t mst_ms, uint16_t mst_step){
}


void midi_controller::on_push(uint8_t btn_id){
	uint16_t pitch;
	lm_.save_n_set(LED_GBR_IDX, btn_id, FOREGROUND2);
	
	if(current_conf_->kb.get_note_offset_C_centered(btn_id, &pitch)
		&& (BIT::is_bit_set(current_conf_->kb_mask[btn_id/KEYBOARD_NR_ROW], (btn_id%KEYBOARD_NR_COL)))){
		_hw_fct((MIDI_C1_OFFSET+pitch), 127, current_conf_->midi_out);
	}
}
void midi_controller::on_release(uint8_t btn_id){
	uint16_t pitch;
	led_toogle* lt = new led_toogle();
	lt->init_animation(&lm_, btn_id, LED_GBR_IDX, FOREGROUND2);
	lt->start_animation(BTN_LED_DELAY_MS);
	btn_animation_list_.add(lt);

	if(current_conf_->kb.get_note_offset_C_centered(btn_id, &pitch)
		&& (BIT::is_bit_set(current_conf_->kb_mask[btn_id/KEYBOARD_NR_ROW], (btn_id%KEYBOARD_NR_COL)))){
		_hw_fct((MIDI_C1_OFFSET+pitch), 0, current_conf_->midi_out);
	}
}
void midi_controller::on_long_release(uint8_t btn_id){
	uint16_t pitch;
	led_toogle* lt = new led_toogle();
	lt->init_animation(&lm_, btn_id, LED_GBR_IDX, FOREGROUND2);
	lt->start_animation(BTN_LED_DELAY_MS);
	btn_animation_list_.add(lt);

	if(current_conf_->kb.get_note_offset_C_centered(btn_id, &pitch) 
		&& (BIT::is_bit_set(current_conf_->kb_mask[btn_id/KEYBOARD_NR_ROW], (btn_id%KEYBOARD_NR_COL)))){
		_hw_fct((MIDI_C1_OFFSET+pitch), 0, current_conf_->midi_out);
	}
}

int midi_controller::menu_on_push(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	UNUSED(func_id);
	UNUSED(opt_id);
	prog::display_title();

	return ret;
}
int midi_controller::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	for(int i=0; i<MATRIX_NR_COL; i++){
		if(i == opt_id){
			get_menu_lm()->save_n_set(LED_R_IDX, func_id * MATRIX_NR_ROW + i, FOREGROUND2);
			current_conf_ = &conf_[opt_id];
		}
		else {
			get_menu_lm()->clr_n_restore(func_id * MATRIX_NR_ROW + i, FOREGROUND2);
		}
	}	
	return ret;
}
void midi_controller::update_ui(uint32_t mst_ms, uint16_t mst_step){
	int len = btn_animation_list_.size();
	
	UNUSED(mst_ms);
	UNUSED(mst_step);

	for(int i=0;i<len;i++){
		led_toogle* lt = btn_animation_list_.shift();
		if(lt->end_animation_n_restore()){
			lt->turn_off_n_restore_led();
			delete lt;
		} else {
			btn_animation_list_.add(lt);
		}
	}
}
