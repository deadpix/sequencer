#include <SoftwareSerial.h>
#include <MIDI.h>
#include "clk.h"
#include "sequencer.h"
#include "track.h"

SoftwareSerial SoftSerial(0, 1);
MIDI_CREATE_INSTANCE(SoftwareSerial, SoftSerial, MIDI);
clk midi_sync;

//sequencer midi_seq;

//void set_midi_sync_clk_ms(uint32_t mst_ms){
//	midi_sync.clk_set_ms(mst_ms);
//}

static void midi_note_on(uint16_t note, uint8_t vel, uint8_t chan){
	MIDI.sendNoteOn(note, vel, chan);
}

void init_midi_seq(sequencer* s){
	track* t;
	s->set_current_track(0);
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		t = s->get_track(i);
		t->set_out_id(i);
		t->init_hw_clbk(midi_note_on);
	}
}

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

	
