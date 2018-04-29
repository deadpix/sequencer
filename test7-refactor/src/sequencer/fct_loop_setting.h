#ifndef __SRC_SEQUENCER_FCT_LOOP_SETTING_H__
#define __SRC_SEQUENCER_FCT_LOOP_SETTING_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "../../fct_clbk.h"
#include "../../sequencer.h"

class fct_loop_setting: public fct_clbk {
	private:
		sequencer* _seq;
		uint8_t end_loop;
			
	public:
		fct_loop_setting(){};
		~fct_loop_setting(){};
		
		void init(sequencer*, char* name);
		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){};
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id);
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
		void on_start();
		void on_leave();
};

#endif
