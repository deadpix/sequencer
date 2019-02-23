#ifndef __SEQ_PARAM_H__
#define __SEQ_PARAM_H__

#include "param.h"
#include "sequencer.h"
#include "track.h"
#include "led_toogle.h"
#include "clk.h"

#define MAX_DIVIDER		8
#define MAX_MULTIPLIER		8

#define SEQ_PARAM_STEP_RED_BTN_ID	0
#define SEQ_PARAM_STEP_BLUE_BTN_ID	1
#define SEQ_PARAM_STEP_GREEN_BTN_ID	2

// Warning: changing these values might crash the system...
#define SEQ_PARAM_LOOP_RED_BTN_ID	 8
#define SEQ_PARAM_LOOP_BLUE_BTN_ID	 9
#define SEQ_PARAM_LOOP_GREEN_BTN_ID	 10
#define SEQ_PARAM_LOOP_RED_BLUE_BTN_ID	 11
#define SEQ_PARAM_LOOP_RED_GREEN_BTN_ID	 12
#define SEQ_PARAM_LOOP_BLUE_GREEN_BTN_ID 13

const uint8_t seq_param_btn_to_color[] = {
  LED_R_IDX,LED_B_IDX,LED_G_IDX,LED_OFF_IDX,LED_OFF_IDX,LED_OFF_IDX,LED_OFF_IDX,LED_OFF_IDX,
  LED_R_IDX,LED_B_IDX,LED_G_IDX,LED_RB_IDX,LED_RG_IDX,LED_GB_IDX,LED_OFF_IDX,LED_OFF_IDX
};

class seq_param: public param {
	private:
		sequencer* _s;
		led_toogle _clk_div_ui[MAX_DIVIDER];
		led_toogle _clk_mul_ui[MAX_MULTIPLIER];
		clk _clk_mul[MAX_MULTIPLIER-1]; 
		clk* _clk_ref;
		void clk_divider_ui(uint32_t, uint16_t);
		void clk_multiplier_ui(uint32_t, uint16_t);

	public:
		seq_param(){};
		~seq_param(){};
		
		void init(sequencer* const s, clk* const c);
		
		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){ UNUSED(btn_id); };
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id);
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
		
		void param_on_enter();
		void param_on_leave();

};

#endif
