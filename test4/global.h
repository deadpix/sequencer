#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "configuration.h"
#include "prog.h"

class global : public prog {
	private:
		
	public:

		boolean play;
	
		global();
		~global();
				
		void on_push(void* ptr, uint8_t btn_id);
		void on_release(void* ptr, uint8_t btn_id);
		
		static led_matrix* menu_clbk(void *, uint8_t, uint8_t, led_matrix*);
		led_matrix* update_menu(uint8_t, uint8_t, led_matrix*);
};

#endif