#include "seq_param.h"

#define CLK_DIVIDER_LED_OFFSET	8
#define LED_ANIMATION_PER		10

//seq_param::seq_param(){
//}
//seq_param::~seq_param(){
//}

void seq_param::clk_divider_ui(uint32_t mst_ms, uint16_t mst_step){
	int i = 0;
	if(mst_ms > 0){
		// new clock
		for(i=0;i<MAX_DIVIDER;i++){
			if(!(mst_step % i)){
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

}

void seq_param::on_push(uint8_t btn_id){
	fct_clbk* fc = _s->get_fct(btn_id);
	param::_lm.toogle_led_x(LED_R_IDX, btn_id);
	if(fc){
		Serial.println(fc->fct_clbk::get_fct_name());
		_s->prog::display_str(fc->get_fct_name(), 1);
	}
	else 
		Serial.println("unknown option");
}

void seq_param::on_release(uint8_t btn_id){
	_s->set_current_param(btn_id);
}

void seq_param::update_ui(uint32_t mst_ms, uint16_t mst_step){
	clk_divider_ui(mst_ms, mst_step);

}
void seq_param::param_on_enter(){
}
void seq_param::param_on_leave(){
	fct_clbk* fc = _s->get_fct(_s->get_current_param());
	if(fc)
		fc->on_start();
}
