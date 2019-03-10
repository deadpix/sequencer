#ifndef __SRC_SEQUENCER_FCT_LOOP_SETTING_H__
#define __SRC_SEQUENCER_FCT_LOOP_SETTING_H__

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif
#include <stdint.h>


#include "../fct_clbk.h"
#include "../sequencer.h"
#include "../led_toogle.h"

struct loop_animation {
	elapsedMillis time_cnt;
	uint16_t led_id;
	bool is_turned_on;
};

class fct_loop_setting: public fct_clbk {
	private:
		sequencer* _seq;
		uint8_t end_loop;
		uint8_t loop_cnt_;
		node* start_loop_;
		node* end_loop_;
//		led_toogle loop_animation_[2];		
		struct loop_animation loop_animation_[2];
	

	public:
		fct_loop_setting(){};
		~fct_loop_setting(){};
		
		void init(sequencer*, char* name);
		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){ UNUSED(btn_id); };
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id);
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
		void on_start();
		void on_leave();
};

#endif
