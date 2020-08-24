#ifndef __PIN_INTERFACE__
#define __PIN_INTERFACE__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif



class PinInterface {
	public:
		virtual uint8_t readPin(uint8_t pin) = 0;
};
#endif