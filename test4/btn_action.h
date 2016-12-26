#ifndef __BTN_ACTION_H__
#define __BTN_ACTION_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "configuration.h"
#include "prog.h"

class btn_action {
	private:
		prog *curr_prog;
	
	public:
		
		void  set_current_prog(prog *);
		prog* get_current_prog();
		
//		void (*btn_on_push)(void* object_ptr, uint8_t btn_id);
//		void (*btn_on_release)(void* object_ptr, uint8_t btn_id);
//		void* object_ptr;
//		void setup_btn_action(void (*)(void*, uint8_t), void (*)(void *, uint8_t), void* ptr);
		
		
//		void (prog::*on_push)(void* object_ptr, uint8_t btn_id);
//		void (prog::*on_release)(void* object_ptr, uint8_t btn_id);
//		void* test_object_ptr;
//		void setup_btn_action_test(void (prog::*)(void*, uint8_t), void (prog::*)(void *, uint8_t), void* ptr);
};


#endif