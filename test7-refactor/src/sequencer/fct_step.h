#ifndef __SRC_SEQUENCER_FCT_STEP_H__
#define __SRC_SEQUENCER_FCT_STEP_H__

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif
#include <stdint.h>


#include "../fct_clbk.h"
#include "../sequencer.h"
#include "../btn_state.h"

//#include <elapsedMillis.h>

#define BTN_MAX_LONG_PUSH_STATE		2

struct led_blink_t {
	elapsedMillis _ms;
	uint8_t _id;
};

class fct_step: public fct_clbk {
	private:
		sequencer* _seq;
		btn_state _bs;
		struct led_blink_t _lp_ui[BTN_MAX_LONG_PUSH_STATE];
		uint8_t _lp_cnt;
	
	public:
		fct_step(){};
		~fct_step(){};
		
		void init(sequencer*, char* name);
		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id);
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
		
		void on_start();
		void on_leave();
};

#endif
