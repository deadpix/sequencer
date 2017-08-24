#ifndef __PROG_H__
#define __PROG_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "led_matrix.h"

class prog {
	protected:
		uint8_t prog_id;
	
	public:
		void set_prog_id(uint8_t);
		uint8_t get_prog_id(void);
		prog* get_prog(void);
		
		virtual void on_push(void* ptr, uint8_t btn_id) = 0;
		virtual void on_release(void* ptr, uint8_t btn_id) = 0;
		virtual led_matrix* get_led_matrix(void);
};


#endif