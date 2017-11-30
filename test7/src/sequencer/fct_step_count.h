#ifndef __SRC_SEQUENCER_FCT_STEP_COUNT_H__
#define __SRC_SEQUENCER_FCT_STEP_COUNT_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "../../fct_clbk.h"
#include "../../sequencer.h"

class fct_step_count: public fct_clbk {
	private:
		sequencer* _seq;
		
	public:
		fct_step_count(){};
		~fct_step_count(){};
		
		void init(sequencer*, char* name);
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);
		void update_ui();
		void on_start();
};

#endif