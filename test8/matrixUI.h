#ifndef __MATRIXUI_H__
#define __MATRIXUI_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "led_matrix.h"

class matrixUI {
	protected:
		uint8_t _mui_id;
		
	public:
		uint8_t get_mui_id(void);
		void set_mui_id(uint8_t);
		matrixUI* get_mui(void);
	
		virtual led_matrix* get_led_matrix(void);
		virtual void on_push(uint8_t btn_id) = 0;
		virtual void on_release(uint8_t btn_id) = 0;
		virtual void update_ui() = 0;
};

#endif
