#ifndef __TEMPO_H__
#define __TEMPO_H__

#include "prog.h"
#include "led_matrix.h"
#include "clk.h"
#include "led_toogle.h"
#include "sequencer.h"

#define NR_TAP	2

class tempo : public prog {
	private:
		led_matrix _lm;
		elapsedMillis _ellapsed_tap;
		clk _mst;
//		uint32_t _tap_timestamp[NR_TAP-1];
		uint32_t _tap_timestamp[NR_TAP];
		uint8_t _tap_cnt;
		led_toogle _tap_animation;
		led_toogle _clk_animation;
		bool _in_menu_mode;
		sequencer* _seq;		
		bool _play;
		
	protected:
		void tap();
	
	public:
		tempo();
		~tempo();

		void init(void (*)(uint32_t), sequencer*, bool);
		
		led_matrix* get_led_matrix(void);
		clk* get_mst_clk();
		void clr_tap(void);
		
		uint32_t check_mst_clk(void);
		
		void menu_enter();
		void menu_leave();
		void menu_update();
		int  menu_on_push(uint8_t, uint8_t);
		int  menu_on_release(uint8_t, uint8_t);
		
		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){ UNUSED(btn_id); };
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id){ UNUSED(btn_id); };
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
};

#endif
