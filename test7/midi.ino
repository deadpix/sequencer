#include <SoftwareSerial.h>
#include <MIDI.h>
#include "clk.h"


SoftwareSerial SoftSerial(0, 1);
MIDI_CREATE_INSTANCE(SoftwareSerial, SoftSerial, MIDI);
clk midi_sync;

//void set_midi_sync_clk_ms(uint32_t mst_ms){
//	midi_sync.clk_set_ms(mst_ms);
//}

void init_midi(){
	MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.turnThruOff();
	
	midi_sync.clk_set_operation(MIDI_SYNC_PPN,midi_sync.clk_get_ms());
//	init_clk(&midi_sync);
}

void midi_loop(boolean flg){
	MIDI.read();
	if(flg){
		MIDI.sendRealTime(MIDI_NAMESPACE::Clock);
	}
}

	
