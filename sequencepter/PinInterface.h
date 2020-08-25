#ifndef __PIN_INTERFACE__
#define __PIN_INTERFACE__

class PinInterface {
	public:
		virtual uint8_t readPin(uint8_t pin) = 0;
};
#endif