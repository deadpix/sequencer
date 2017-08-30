#include "led_toogle.h"

#define STARTED			true
#define STOPPED			false

led_toogle::led_toogle(led_matrix* lm, uint16_t led_id, uint8_t color){
	_lm = lm;
	_led_id = led_id;
	_color = color;
	_state = STOPPED;
}
led_toogle::~led_toogle(){
}

int led_toogle::start_animation(uint32_t ms, boolean init_led_state, boolean mode){
	int ret = 0;
	
	_animation_time = ms;
	_animation_mode = _mode;
	_state = STARTED;
	if(init_led_state == LED_ON){
		_lm->set_led_x(_color, _led_id);
	} 
	else {
		_lm->clr_led_x(_color, _led_id);		
	}
	_time_cnt = 0;
	
	return ret;
}

int led_toogle::update_animation(boolean hold){
	int ret = 0;
	
	if(_state == STARTED){
		if(hold){
			_time_cnt = 0;
		}
		else if(_time_cnt >= _animation_time){
			_lm->toogle_led_x(_color, _led_id);
			_time_cnt = 0;
			ret = 1;
			if(_animation_mode = MODE_ONE_SHOT)
				_state = STOPPED;
		}
	}
	
	return ret;
}

int led_toogle::stop_animation(){
	_state = STOPPED;
}