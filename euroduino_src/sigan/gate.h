#ifndef _LIB_GATE_H_
#define _LIB_GATE_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include <elapsedMillis.h>

#define GATE_HIGH	true
#define GATE_LOW	false

#define GATE_ERROR	0
#define GATE_STARTED	1
#define GATE_FINISHED	2

class gate {
	private:
		bool _gate_value;
		uint32_t _gate_len;
		elapsedMillis _elapsed_ms;
		uint8_t _port;
		int	_gate_state;	

	public:
		gate();
		gate(uint8_t, uint8_t (*hw_wr)(uint8_t, bool));
		~gate();
	
//		void set_gate_value(boolean);
		void set_gate_len(uint32_t);
		void set_hw_cbck(uint8_t, uint8_t (*hw_wr)(uint8_t, bool));	
	
		int rst_gate(bool);
		int upd_gate();

		
};
#endif
