#ifndef _LIB_CLK_H_
#define _LIB_CLK_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include <elapsedMillis.h>

class clk {
	private:
		uint16_t _step_cnt;
		uint8_t	 _max_step;
		uint16_t _bpm;
		uint32_t _ms; 
		elapsedMillis _elapsed_ms;
		// diff from 0 and less than -1: divider
		// diff from 0 and sup than 1: multiplier
		int _operation; 
		boolean _resync;
		
		void _sync_intern(uint32_t);
	
	public:
		clk();
		~clk();
		
		uint32_t clk_get_ms();
		uint16_t clk_get_step_cnt();
		uint32_t clk_get_elapsed_ms();

		int clk_set_ms(uint32_t);
		int clk_set_bpm(uint16_t);	
	
		void clk_set_max_step(uint8_t);
		
		boolean clk_set_operation(int);
		void clk_sync_extern();
		
		boolean clk_next_step();
		uint32_t clk_elapsed();
		void clk_sync_divider(uint32_t, uint16_t);
		void clk_sync_multiplier(uint32_t);
		boolean clk_update(boolean, clk*);
		void clk_sync_slaved(uint16_t);
};

#endif
