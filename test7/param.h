#ifndef __PARAM_H__
#define __PARAM_H__


//class prog;

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "led_matrix.h"
#include "prog.h"

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif

class param: public prog {
	
	protected:
		led_matrix  _lm;
		prog* _p;
	
	public:
		
//		param();
//		~param();

		led_matrix* get_led_matrix(void);
		prog* get_prog();
		void set_prog(prog*);
		
		// dummy declaration
		void menu_enter(){};
		void menu_leave(){};
		void menu_update(){};
		int  menu_on_push(uint8_t, uint8_t){return 1;}
		int  menu_on_release(uint8_t, uint8_t){return 1;}
	
		virtual void on_push(uint8_t btn_id) = 0;
		virtual void on_long_push(uint8_t btn_id) = 0;
		virtual void on_release(uint8_t btn_id) = 0;
		virtual void on_long_release(uint8_t btn_id) = 0;
		virtual void update_ui(uint32_t mst_ms, uint16_t mst_step) = 0;
		
		virtual void param_on_enter() = 0;
		virtual void param_on_leave() = 0;
};


#endif
