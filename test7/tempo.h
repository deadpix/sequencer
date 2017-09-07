#ifndef __TEMPO_H__
#define __TEMPO_H__

#include "prog.h"
#include "led_matrix.h"
#include "clk.h"
#include "led_toogle.h"

#define NR_TAP	2

class tempo : public prog {
	private:
		led_matrix _lm;
		elapsedMillis _ellapsed_tap;
		clk _mst;
		uint32_t _tap_timestamp[NR_TAP-1];
		uint8_t _tap_cnt;
		led_toogle _tap_animation;
		led_toogle _clk_animation;
		
	protected:
		void tap();
	
	public:
		tempo();
		~tempo();

		void init();
		led_matrix* get_led_matrix(void);
		void clr_tap(void);
		
		uint32_t check_mst_clk(void);
		
		void menu_enter();
		void menu_leave();
		void menu_update();
		int  menu_on_push(uint8_t, uint8_t/*, led_matrix**/);
		int  menu_on_release(uint8_t, uint8_t/*, led_matrix**/);
		
		void on_push(/*void* ptr, */uint8_t btn_id);
		void on_release(/*void* ptr, */uint8_t btn_id);
		
//		static led_matrix* clbk_menu_on_push(void *, uint8_t, uint8_t, led_matrix*);
//		static led_matrix* clbk_menu_on_release(void *, uint8_t, uint8_t, led_matrix*);
};

#endif