#ifndef __TEMPO_H__
#define __TEMPO_H__

#include "prog.h"
#include "led_matrix.h"
#include "clk.h"

#define NR_TAP	2

class tempo : public prog {
	private:
		led_matrix _lm;
		ellapsed_millis _ellapsed_tap;
		clk _mst;
		uin32_t _tap_timestamp[NR_TAP-1];
		uint8_t _tap_cnt;
		
	protected:
		void tap();
	
	public:
		tempo();
		~tempo();

		led_matrix* get_led_matrix(void);
		void clr_tap(void);
		
		void on_push(void* ptr, uint8_t btn_id);
		void on_release(void* ptr, uint8_t btn_id);
		
		static led_matrix* clbk_menu_on_push(void *, uint8_t, uint8_t, led_matrix*);
		static led_matrix* clbk_menu_on_release(void *, uint8_t, uint8_t, led_matrix*);
		led_matrix* menu_on_push(uint8_t, uint8_t, led_matrix*);
		led_matrix* menu_on_release(uint8_t, uint8_t, led_matrix*);
}

#endif