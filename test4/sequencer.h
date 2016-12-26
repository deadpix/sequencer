#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "configuration.h"
#include "prog.h"
#include "seq.h"
#include "clock.h"
#include "led_matrix.h"

#define SEQUENCER_NR_PART NUM_LED_COLUMNS

class sequencer : public prog {
	private:
		seq seq_arr[SEQUENCER_NR_PART];
		seq* current;
		
	public:
		static const uint8_t nr_seq = 1;

		sequencer();
		~sequencer();
		
		seq* get_seq(uint8_t seq_id);
		seq* get_current_seq();
		void set_current_seq(uint8_t seq_id);
		
		void on_push(void* ptr, uint8_t btn_id);
		void on_release(void* ptr, uint8_t btn_id);
		
		static led_matrix* menu_clbk(void *, uint8_t, uint8_t, led_matrix*);
		led_matrix* update_menu(uint8_t, uint8_t, led_matrix*);

		void update_out(clock*);
		void update_gui();
};

#endif