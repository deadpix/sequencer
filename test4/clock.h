#ifndef _CLOCK_H
#define _CLOCK_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "led_matrix.h"

#define NR_SLAVES 	1

class clock {
	private:
		uint16_t step;
		uint16_t bpm;
		uint8_t gate_len;
		elapsedMillis elapsed_ms;
		uint32_t interval_ms; 
		uint32_t gate_ms; 
		int factor;
		boolean resync;
		boolean gui_state;
		
		void sync_intern(uint32_t);
	
	public:
		clock();
		~clock();
		
		uint32_t getInterval();
		uint16_t getStep();
		uint32_t getGateLen();
		uint32_t getElapsedMs();
		
		void setFactor(int);
		void sync_extern();
		
		boolean clock_next_step(boolean*);
		boolean clock_get_gate_status(void);
		uint8_t clock_get_gate_status2();
		void clock_sync_divider(uint32_t, uint16_t);
		void clock_sync_multiplier(uint32_t);
		boolean clock_update(boolean, clock*);
		void clock_upd_gui(uint8_t, led_matrix *, boolean);
};

#endif
