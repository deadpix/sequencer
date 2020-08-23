/*
  The MIT License (MIT)

  Copyright (c) 2013 thomasfredericks

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
  Main code by Thomas O Fredericks (tof@t-o-f.info)
  Previous contributions by Eric Lowry, Jim Schimpf and Tom Harkaway
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Bounce_array_h
#define Bounce_array_h


#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif



// Uncomment the following line for "LOCK-OUT" debounce method
//#define BOUNCE_LOCK_OUT

// Uncomment the following line for "BOUNCE_WITH_PROMPT_DETECTION" debounce method
//#define BOUNCE_WITH_PROMPT_DETECTION

#include <inttypes.h>
#include "PinInterface.hpp"

class ArrBounce
{
 public:
    // Create an instance of the bounce library
    ArrBounce();

    void attach(uint8_t* pin_arr, PinInterface* interface);

    // Attach to a pin (and also sets initial state)
    void attach(int pin, int idx);
    
    // Attach to a pin (and also sets initial state) and sets pin to mode (INPUT/INPUT_PULLUP/OUTPUT)
    void attach(int pin, int mode, int idx);

    // Sets the debounce interval
    void interval(uint16_t interval_millis);

    // Updates the pin
    // Returns 1 if the state changed
    // Returns 0 if the state did not change
    bool update(int idx);

    // Returns the updated pin state
    bool read(int idx);

    // Returns the falling pin state
    bool fell(int idx);

    // Returns the rising pin state
    bool rose(int idx);

	  void init_ArrBounce(uint8_t* pin_arr, unsigned long interval_millis, int size, PinInterface* interface);
	
    // Partial compatibility for programs written with Bounce version 1
    bool risingEdge(int idx) { return rose(idx); }
    bool fallingEdge(int idx) { return fell(idx); }

    ArrBounce(uint8_t* pin_arr, unsigned long interval_millis, int size, PinInterface* interface) : ArrBounce() {
		  this->size = size;
		  interval(interval_millis);
		  previous_millis = new unsigned long int[size];
		  state = new unsigned char[size];
      pin = new unsigned char[size];
		  attach(pin_arr, interface);
	  }

 protected:
    unsigned long* previous_millis;
    uint16_t interval_millis;
    uint8_t* state;
    uint8_t* pin;
	  int size;
    PinInterface* interface;
};

#endif
