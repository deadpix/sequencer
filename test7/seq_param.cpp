#include "seq_param.h"

#define CLK_DIVIDER_LED_OFFSET		8
#define CLK_MULTIPLIER_LED_OFFSET	16
#define LED_ANIMATION_PER		20

//seq_param::seq_param(){
//}
//seq_param::~seq_param(){
//}

void seq_param::clk_divider_ui(uint32_t mst_ms, uint16_t mst_step){
	int i = 0;
	if(mst_ms > 0){
		// new clock
		for(i=0;i<MAX_DIVIDER;i++){
			if(!(mst_step % (i+1))){
				// start led animation
				_clk_div_ui[i].turn_on_led();
				_clk_div_ui[i].start_animation(LED_ANIMATION_PER * mst_ms / 100);
			}
		}
	}
	
	// update led aimation
	for(i=0;i<MAX_DIVIDER;i++){
		_clk_div_ui[i].update_animation();
	}
}

void seq_param::clk_multiplier_ui(uint32_t mst_ms, uint16_t mst_step){
//_s->get_current_track()->get_clk()->clk_get_ms()
	if(mst_ms > 0){
//		reset and update clock ms reference
		_clk_mul_ui[0].turn_on_led();
		_clk_mul_ui[0].start_animation(LED_ANIMATION_PER * mst_ms / 100);

		for(int i=0;i<(MAX_MULTIPLIER-1);i++){
			_clk_mul[i].clk_sync(mst_ms, mst_step);
			_clk_mul_ui[i+1].turn_on_led();
			_clk_mul_ui[i+1].start_animation(LED_ANIMATION_PER * _clk_mul[i].clk_get_ms() / 100);
		}
	}
	else {
		for(int i=0;i<(MAX_MULTIPLIER-1);i++){
			uint32_t ms = _clk_mul[i].clk_elapsed();
			if(ms > 0){
				_clk_mul_ui[i+1].turn_on_led();
				_clk_mul_ui[i+1].start_animation(LED_ANIMATION_PER * ms / 100);
			}
		}
	}
	for(int i=0;i<MAX_MULTIPLIER;i++){
		_clk_mul_ui[i].update_animation();
	}
}

void seq_param::init(sequencer* const s){
	_s = s;
	for(int i=0;i<(MATRIX_NR_COL*MATRIX_NR_ROW);i++){
		if(_s->get_fct(i))
			param::_lm.set_led_x(LED_R_IDX, i);
	}
	if(_s->get_fct(_s->current_param_id))
		param::_lm.set_led_x(LED_GBR_IDX, _s->current_param_id);

	for(int i=0;i<MAX_DIVIDER;i++){
		_clk_div_ui[i].init_animation(param::get_led_matrix(),(CLK_DIVIDER_LED_OFFSET+i), LED_R_IDX);
	}
	
	_clk_mul_ui[0].init_animation(param::get_led_matrix(),(CLK_MULTIPLIER_LED_OFFSET), LED_R_IDX);
	for(int i=1;i<MAX_MULTIPLIER;i++){
		_clk_mul_ui[i].init_animation(param::get_led_matrix(),(CLK_MULTIPLIER_LED_OFFSET+i), LED_R_IDX);
		_clk_mul[(i-1)].clk_set_operation((i+1), _s->get_current_track()->get_clk()->clk_get_ms());
	}

}

void seq_param::on_push(uint8_t btn_id){
	fct_clbk* fc = _s->get_fct(btn_id);
	if(fc){
		_s->prog::display_str(fc->get_fct_name(), 1);
		param::_lm.clr_led_x(LED_GB_IDX, _s->get_current_param());
		param::_lm.set_led_x(LED_GBR_IDX, btn_id);
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

void seq_param::update_ui(uint32_t mst_ms, uint16_t mst_step){
	clk_divider_ui(mst_ms, mst_step);
	clk_multiplier_ui(mst_ms, mst_step);
}
void seq_param::param_on_enter(){
	Serial.print("param on enter ");
	Serial.println(_s->get_current_param());
}
void seq_param::param_on_leave(){
	fct_clbk* fc = _s->get_fct(_s->get_current_param());
	if(fc)
		fc->on_start();
}
