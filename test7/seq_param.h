#ifndef __SEQ_PARAM_H__
#define __SEQ_PARAM_H__

#include "param.h"
#include "sequencer.h"
#include "track.h"
#include "led_toogle.h"
#include "clk.h"

#define MAX_DIVIDER		8
#define MAX_MULTIPLIER		8

class seq_param: public param {
	private:
		sequencer* _s;
		led_toogle _clk_div_ui[MAX_DIVIDER];
		led_toogle _clk_mul_ui[MAX_MULTIPLIER];
		clk _clk_mul[MAX_MULTIPLIER-1]; 

		void clk_divider_ui(uint32_t, uint16_t);
		void clk_multiplier_ui(uint32_t, uint16_t);

	public:
		seq_param(){};
		~seq_param(){};
		
		void init(sequencer* const s);
		
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
		
		void param_on_enter();
		void param_on_leave();

};

#endif
