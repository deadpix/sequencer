#ifndef __FCT_STEP_H__
#define __FCT_STEP_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "fct_clbk.h"
#include "sequencer.h"

class fct_step: public fct_clbk {
	private:
		sequencer* _seq;
		
	
	public:
		fct_step(){};
		~fct_step(){};
		
		void init(sequencer*);		
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);
		void update_ui();
};

#endif