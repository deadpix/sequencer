#include "seq_param.h"
#include <hw_debug.h>

#define CLK_DIVIDER_LED_OFFSET		8
#define CLK_MULTIPLIER_LED_OFFSET	16
#define LED_ANIMATION_PER		20

void seq_param::clk_divider_ui(uint32_t mst_ms, uint16_t mst_step){
	int i = 0;

	if(mst_ms > 0){
		// new clock
		for(i=0;i<MAX_DIVIDER;i++){
			if(!(mst_step % (i+1))){
				// start led animation
				_clk_div_ui[i].turn_on_n_save_led();
				_clk_div_ui[i].start_animation(LED_ANIMATION_PER * mst_ms / 100);
			}
		}
	}
	
	// update led aimation
	for(i=0;i<MAX_DIVIDER;i++){
		_clk_div_ui[i].end_animation_n_restore();	
	}

}

void seq_param::clk_multiplier_ui(uint32_t mst_ms, uint16_t mst_step){
	if(mst_ms > 0){
//		reset and update clock ms reference
		_clk_mul_ui[0].turn_on_n_save_led();
		_clk_mul_ui[0].start_animation(LED_ANIMATION_PER * mst_ms / 100);

		for(int i=0;i<(MAX_MULTIPLIER-1);i++){
			_clk_mul[i].clk_sync_ratio(mst_ms, mst_step);
			_clk_mul_ui[i+1].turn_on_n_save_led();
			_clk_mul_ui[i+1].start_animation(LED_ANIMATION_PER * _clk_mul[i].clk_get_ms() / 100);
		}
	}
	else {
		for(int i=0;i<(MAX_MULTIPLIER-1);i++){
			uint32_t ms = _clk_mul[i].clk_elapsed();
			if(ms > 0){
				_clk_mul_ui[i+1].turn_on_n_save_led();
				_clk_mul_ui[i+1].start_animation(LED_ANIMATION_PER * ms / 100);
			}
		}
	}
	for(int i=0;i<MAX_MULTIPLIER;i++){
		_clk_mul_ui[i].end_animation_n_restore();
	}
}

void seq_param::init(sequencer* const s, clk* const c){
	_s = s;
	_clk_ref = c;
	int ret;
	for(int i=0;i<(MATRIX_NR_COL*MATRIX_NR_ROW);i++){
		if(_s->get_fct(i)){
			param::_lm.save_n_set(LED_R_IDX, i, FOREGROUND2);
		}
	}
	if(_s->get_fct(_s->current_param_id)){
		ret = param::_lm.save_n_ovw(LED_GBR_IDX, _s->current_param_id, FOREGROUND2);
	}

	for(int i=0;i<MAX_DIVIDER;i++){
		_clk_div_ui[i].init_animation(param::get_led_matrix(),(CLK_DIVIDER_LED_OFFSET+i), LED_R_IDX);
	}
	
	_clk_mul_ui[0].init_animation_n_save(param::get_led_matrix(),(CLK_MULTIPLIER_LED_OFFSET), LED_R_IDX);
	for(int i=1;i<MAX_MULTIPLIER;i++){
		_clk_mul_ui[i].init_animation(param::get_led_matrix(),(CLK_MULTIPLIER_LED_OFFSET+i), LED_R_IDX);
		_clk_mul[(i-1)].clk_set_ratio(_clk_ref->clk_get_ms(), 1, (i+1));
	}

}

void seq_param::on_push(uint8_t btn_id){
	fct_clbk* fc = _s->get_fct(btn_id);
	if(fc){
		_s->prog::display_str(fc->get_fct_name(), 1);
		param::_lm.save_n_ovw(LED_R_IDX, _s->get_current_param(), FOREGROUND2);
		param::_lm.save_n_ovw(LED_GBR_IDX, btn_id, FOREGROUND2);
		dbg::printf("passe");
	}
	else if(btn_id >= CLK_DIVIDER_LED_OFFSET && btn_id < CLK_MULTIPLIER_LED_OFFSET){ 
//		_s->get_current_track()->_clk_def.numerator = (btn_id-CLK_DIVIDER_LED_OFFSET+1);
//		_s->get_current_track()->_clk_def.denominator = 1;	
		
		_s->get_current_track()->set_clk_def_lock((btn_id-CLK_DIVIDER_LED_OFFSET+1), 1);
		_s->prog::display_str("div", 1);
	}
	else if(btn_id >= CLK_MULTIPLIER_LED_OFFSET && btn_id < (CLK_MULTIPLIER_LED_OFFSET + 8)){
//		_s->get_current_track()->_clk_def.numerator = 1;
//		_s->get_current_track()->_clk_def.denominator = (btn_id-CLK_MULTIPLIER_LED_OFFSET+1);
		_s->get_current_track()->set_clk_def_lock(1, (btn_id-CLK_MULTIPLIER_LED_OFFSET+1));

		_s->prog::display_str("mult", 1);
	}
	else {
		_s->prog::display_str("undef", 1);
	}
}

void seq_param::on_release(uint8_t btn_id){
	fct_clbk* fc = _s->get_fct(btn_id);
	if(fc){
		_s->set_current_param(btn_id);
	}
}

void seq_param::on_long_release(uint8_t btn_id){
	UNUSED(btn_id);
}

void seq_param::update_ui(uint32_t mst_ms, uint16_t mst_step){
	clk_divider_ui(mst_ms, mst_step);
	clk_multiplier_ui(mst_ms, mst_step);
}
void seq_param::param_on_enter(){
	fct_clbk* fc = _s->get_fct(_s->get_current_param());
	if(fc){
		fc->on_leave();
	}
}
void seq_param::param_on_leave(){
	fct_clbk* fc = _s->get_fct(_s->get_current_param());
	if(fc){
		fc->on_start();
	}
}
