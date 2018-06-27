#ifndef __LED_TOOGLE_H__
#define __LED_TOOGLE_H__

//#include "led_animation.h"
#include <elapsedMillis.h>
#include "led_matrix.h"

#define LED_ON			true
#define LED_OFF			false
#define MODE_ONE_SHOT	true


class led_toogle {
	private:
		elapsedMillis _time_cnt;
		led_matrix* _lm;
		bool _state;
		uint16_t _led_id;
		uint8_t _color;
		uint8_t _ground;
		uint32_t _animation_time;
//		boolean _animation_mode;

		
		
	public:
		led_toogle();
		led_toogle(led_matrix* lm, uint16_t led_id, uint8_t color, uint8_t ground);
		~led_toogle();
		
//		void turn_on_led(void);
//		void turn_off_led(void);
		void turn_on_n_save_led(void);
		void turn_off_n_restore_led(void);
//		void menu_update(void);

		void init_animation(led_matrix* lm, uint16_t led_id, uint8_t color);
		int start_animation(uint32_t/*, boolean*//*, boolean*/);
		int update_animation(/*boolean hold*/);
		int stop_animation();
		void init_animation_n_save(led_matrix* lm, uint16_t led_id, uint8_t color);
		int end_animation_n_restore();
};

#endif
