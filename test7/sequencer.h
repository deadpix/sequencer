#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "types.h"
#include "prog.h"
#include "clk.h"
#include "led_matrix.h"
#include "track.h"

#define SEQUENCER_NR_TRACK MATRIX_NR_COL

class sequencer : public prog {
	private:
		track  track_arr[SEQUENCER_NR_TRACK];
		track* current;
		
	public:
		static const uint8_t nr_track = 1;

		sequencer();
		~sequencer();
		
		track* get_track(uint8_t track_id);
		track* get_current_track();
		void set_current_track(uint8_t track_id);
		
		led_matrix* get_led_matrix(void);

		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);
		void update_ui();
	
		void check_clks(uint32_t mst_ms, uint16_t mst_step);
				
		void menu_enter();
		void menu_leave();
		void menu_update();
		int  menu_on_push(uint8_t, uint8_t);
		int  menu_on_release(uint8_t, uint8_t);		
};

#endif