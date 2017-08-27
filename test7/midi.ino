#include <SoftwareSerial.h>
#include <MIDI.h>

// Pulse Per Quarter Note (PPQN)
#define MIDI_SYNC_PPQN	24
// Pulse Per Note (PPN)
#define MIDI_SYNC_PPN	(MIDI_SYNC_PPQN*4)

SoftwareSerial SoftSerial(0, 1);
MIDI_CREATE_INSTANCE(SoftwareSerial, SoftSerial, MIDI);

void init_midi(){
	MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.turnThruOff();
}

void midi_loop(boolean flg){
	MIDI.read();
	if(flg){
		MIDI.sendRealTime(MIDI_NAMESPACE::Clock);
	}
}

	
