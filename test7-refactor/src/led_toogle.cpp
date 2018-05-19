#include "led_toogle.h"

#define STARTED			true
#define STOPPED			false

led_toogle::led_toogle(){	
}

led_toogle::led_toogle(led_matrix* lm, uint16_t led_id, uint8_t color){
	_lm = lm;
	_led_id = led_id;
	_color = color;
	_state = STOPPED;
}
led_toogle::~led_toogle(){
}

void led_toogle::turn_on_led(){
	_lm->set_led_x(_color, _led_id);
}
void led_toogle::turn_off_led(){
	_lm->clr_led_x(_color, _led_id);
}
void led_toogle::turn_on_n_save_led(){
	_lm->save_n_set(_color, _led_id, FOREGROUND2);
}
void led_toogle::turn_off_n_restore_led(){
	_lm->clr_n_restore(_led_id, FOREGROUND2);
}



void led_toogle::init_animation(led_matrix* lm, uint16_t led_id, uint8_t color){
	_lm = lm;
	_led_id = led_id;
	_color = color;
	_state = STOPPED;	
}


int led_toogle::start_animation(uint32_t ms){
	int ret = 0;	
	_animation_time = ms;
	_state = STARTED;
	_time_cnt = 0;
	
	return ret;
}

int led_toogle::update_animation(){
	int ret = 0;
	
	if(_state == STARTED){
		if(_time_cnt >= _animation_time){
			_lm->toogle_led_x(_color, _led_id);
			_time_cnt = 0;
			ret = 1;
			_state = STOPPED;
		}
	}
	
	return ret;
}
void led_toogle::init_animation_n_save(led_matrix* lm, uint16_t led_id, uint8_t color){
	_lm = lm;
	_led_id = led_id;
	_color = color;
	_state = STOPPED;
	turn_on_n_save_led();
}

int led_toogle::end_animation_n_restore(){
	int ret = 0;
	
	if(_state == STARTED){
		if(_time_cnt >= _animation_time){
			turn_off_n_restore_led();
			_time_cnt = 0;
			ret = 1;
			_state = STOPPED;
		}
	}
	
	return ret;
}

int led_toogle::stop_animation(){
	_state = STOPPED;
}
