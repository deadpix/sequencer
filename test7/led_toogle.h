#ifndef __LED_TOOGLE_H__
#define __LED_TOOGLE_H__

//#include "led_animation.h"
#include <elapsedMillis.h>

#define LED_ON			true
#define LED_OFF			false
#define MODE_ONE_SHOT	true


class led_toogle {
	private:
		ellapsed_millis _time_cnt;
		led_matrix* _lm;
		boolean _state;
		uint16_t _led_id;
		uint8_t _color;
		uint32_t _animation_time;
		boolean _animation_mode;

	public:
		led_toogle(led_matrix*, uint16_t, uint8_t);
		~led_toogle();
		
		int start_animation(uint32_t, boolean, boolean);
		int update_animation(boolean hold);
		int stop_animation();
};

#endif