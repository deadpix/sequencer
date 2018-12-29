#ifndef __LED_STATUS_H__
#define __LED_STATUS_H__

class led_state {
	public:
		uint8_t _color;
		uint8_t _prio;
		bool _in_use;
		led_state(){ 
			_in_use = false; 
		}	
		led_state(uint8_t color, uint8_t prio){
			_color = color;
			_prio = prio;
			_in_use = false; 
		}
		~led_state(){};
};

#endif
