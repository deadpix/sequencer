// Please read Bounce2.h for information about the liscence and authors

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Bounce_array.h"

#define DEBOUNCED_STATE 0
#define UNSTABLE_STATE  1
#define STATE_CHANGED   3

ArrBounce::ArrBounce()
    : previous_millis(0)
    , interval_millis(10)
    , state(0)
    , pin(0)
{}

void ArrBounce::init_ArrBounce(uint8_t* pin_arr, unsigned long interval_millis, int size, PinInterface* interface){
	this->size = size;
	interval(interval_millis);
	previous_millis = new unsigned long int[size];
	state = new unsigned char[size];
	pin = new unsigned char[size];
	attach(pin_arr, interface);
}


void ArrBounce::attach(uint8_t* pin_arr, PinInterface* interface) {
	this->interface = interface;
	for(int i=0;i<this->size;i++){
		this->attach(pin_arr[i], i);
	}
}



void ArrBounce::attach(int pin, int idx) {
    this->pin[idx] = pin;
    state[idx] = 0;
//    if (digitalRead(pin)) {
    if (interface->readPin(pin)) {
        state[idx] = _BV(DEBOUNCED_STATE) | _BV(UNSTABLE_STATE);
    }
#ifdef BOUNCE_LOCK_OUT
    previous_millis[idx] = 0;
#else
    previous_millis[idx] = millis();
#endif
}

void ArrBounce::attach(int pin, int mode, int idx){
  pinMode(pin, mode);
  
  this->attach(pin, idx);
}

void ArrBounce::interval(uint16_t interval_millis)
{
    this->interval_millis = interval_millis;
}

bool ArrBounce::update(int idx)
{
#ifdef BOUNCE_LOCK_OUT
    state[idx] &= ~_BV(STATE_CHANGED);
    // Ignore everything if we are locked out
    if (millis() - previous_millis[idx] >= interval_millis) {
//        bool currentState = digitalRead(pin[idx]);
		bool currentState = interface->readPin(pin[idx]);
        if ((bool)(state[idx] & _BV(DEBOUNCED_STATE)) != currentState) {
            previous_millis[idx] = millis();
            state[idx] ^= _BV(DEBOUNCED_STATE);
            state[idx] |= _BV(STATE_CHANGED);
        }
    }
    return state[idx] & _BV(STATE_CHANGED);

#elif defined BOUNCE_WITH_PROMPT_DETECTION
    // Read the state of the switch port into a temporary variable.

//	bool readState = digitalRead(pin[idx]);
	bool readState = interface->readPin(pin[idx]);


    // Clear Changed State Flag - will be reset if we confirm a button state change.
    state[idx] &= ~_BV(STATE_CHANGED);

    if ( readState != (bool)(state[idx] & _BV(DEBOUNCED_STATE))) {
      // We have seen a change from the current button state.

      if ( millis() - previous_millis[idx] >= interval_millis ) {
	// We have passed the time threshold, so a new change of state is allowed.
	// set the STATE_CHANGED flag and the new DEBOUNCED_STATE.
	// This will be prompt as long as there has been greater than interval_misllis ms since last change of input.
	// Otherwise debounced state will not change again until bouncing is stable for the timeout period.
	state[idx] ^= _BV(DEBOUNCED_STATE);
	state[idx] |= _BV(STATE_CHANGED);
      }
    }

    // If the readState is different from previous readState, reset the debounce timer - as input is still unstable
    // and we want to prevent new button state changes until the previous one has remained stable for the timeout.
    if ( readState != (bool)(state[idx] & _BV(UNSTABLE_STATE)) ) {
	// Update Unstable Bit to macth readState
        state[idx] ^= _BV(UNSTABLE_STATE);
        previous_millis[idx] = millis();
    }
    // return just the sate changed bit
    return state[idx] & _BV(STATE_CHANGED);
#else

    // Read the state of the switch in a temporary variable.
//    bool currentState = digitalRead(pin[idx]);
    bool currentState = interface->readPin(pin[idx]);

 //	Serial.print("digitalRead(pin[idx]) " );
 	//Serial.println(digitalRead(pin[idx]));

	
	
	//    bool currentState = dig_read(pin[idx]);
    state[idx] &= ~_BV(STATE_CHANGED);

    // If the reading is different from last reading, reset the debounce counter
    if ( currentState != (bool)(state[idx] & _BV(UNSTABLE_STATE)) ) {
        previous_millis[idx] = millis();
        state[idx] ^= _BV(UNSTABLE_STATE);
    } else
        if ( millis() - previous_millis[idx] >= interval_millis ) {
            // We have passed the threshold time, so the input is now stable
            // If it is different from last state, set the STATE_CHANGED flag
            if ((bool)(state[idx] & _BV(DEBOUNCED_STATE)) != currentState) {
                previous_millis[idx] = millis();
                state[idx] ^= _BV(DEBOUNCED_STATE);
                state[idx] |= _BV(STATE_CHANGED);
            }
        }

    return state[idx] & _BV(STATE_CHANGED);
#endif
}

bool ArrBounce::read(int idx)
{
    return state[idx] & _BV(DEBOUNCED_STATE);
}

bool ArrBounce::rose(int idx)
{
    return ( state[idx] & _BV(DEBOUNCED_STATE) ) && ( state[idx] & _BV(STATE_CHANGED));
}

bool ArrBounce::fell(int idx)
{
    return !( state[idx] & _BV(DEBOUNCED_STATE) ) && ( state[idx] & _BV(STATE_CHANGED));
}
