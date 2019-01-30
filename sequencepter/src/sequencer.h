#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif
#include <stdint.h>

#include "types.h"
#include "prog.h"
#include "clk.h"
#include "led_matrix.h"
#include "track.h"
#include "fct_clbk.h"
#include "event.h"

//#define SEQUENCER_NR_TRACK MATRIX_NR_COL
#define SEQUENCER_NR_TRACK 8

//typedef struct _transport {
//	byte play;
//} transport_t;

struct linked_step_ui {
	elapsedMillis ms_cnt;
	int step_id;
};

class sequencer : public prog {
	private:
		track  track_arr[SEQUENCER_NR_TRACK];
		track* current;
		fct_clbk* fct_arr[MATRIX_NR_COL*MATRIX_NR_ROW];
//		transport_t _transport;
		
	public:
		static const uint8_t nr_track = 1;
		uint8_t current_param_id;

		struct linked_step_ui _ls_ui;
		
		sequencer();
		~sequencer();
		
		track* get_track(uint8_t track_id);
		track* get_current_track();
		void set_current_track(uint8_t track_id);
		void set_current_param(uint8_t);
		uint8_t get_current_param();
		
		void set_track_start(bool);
		void reset_all();	
		
		void add_fct(fct_clbk*, uint8_t);
		fct_clbk* get_fct(uint8_t);
		
		led_matrix* get_led_matrix(void);

		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id);
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
	
		uint8_t check_events(uint32_t mst_ms, uint16_t mst_step, event** e);
				
		void menu_enter();
		void menu_leave();
		void menu_update(uint32_t mst_ms, uint16_t mst_step);
		int  menu_on_push(uint8_t, uint8_t);
		int  menu_on_release(uint8_t, uint8_t);		
};

class next_step_evt : public event {
	private:
		sequencer* _s;

	public:
		uint8_t step_evt_bmp;
		next_step_evt(sequencer* s, uint8_t bmp){_s = s; step_evt_bmp = bmp; };
		~next_step_evt(){};
		void do_evt();
};
#endif
