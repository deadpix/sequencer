#include <SoftwareSerial.h>
#include <MIDI.h>
#include "src/clk.h"
#include "src/sequencer.h"
#include "src/track.h"
#include "src/test_proj_one.h"
#include "src/hw.h"

#include "src/midi_cst.h"

SoftwareSerial SoftSerial(0, 1);
MIDI_CREATE_INSTANCE(SoftwareSerial, SoftSerial, MIDI);
clk midi_sync;

/* MPC
Pad Bank A :
49 (C#2) 55 (G2) 51 (D#2) 53 (F2)
48 (C2) 47 (B1) 45 (A1) 43 (G1)
40 (E1) 38 (D1) 46 (A#1) 44 (G#1)
37 (C#1) 36 (C1) 42 (F#1) 82 (A#4)

Pad Bank B :
73 (C#4) 74 (D4) 71 (B3) 39 (D#1)
56 (G#2) 62 (D3) 63 (D#3) 64 (E3)
65 (F3) 66 (F#3) 76 (E4) 77 (F4)
54 (F#2) 69 (A3) 81 (A4) 80 (G#4)

Pad Bank C :
79 (G4) 35 (B0) 41 (F1) 50 (D2)
70 (A#3) 72 (C4) 75 (D#4) 78 (F#4)
60 (C3) 61 (C#3) 67 (G3) 68 (G#3)
52 (E2) 57 (A3) 58 (A#2) 59 (B2)

Pad Bank D :
95 (B5) 96 (C6) 97 (C#6) 98 (D6)
91 (G5) 92 (G#5) 93 (A5) 94 (A#5)
87 (D#5) 88 (E5) 89 (F5) 90 (F#5)
83 (B4) 84 (C5) 85 (C#5) 86 (D5)
*/

//sequencer midi_seq;

//void set_midi_sync_clk_ms(uint32_t mst_ms){
//	midi_sync.clk_set_ms(mst_ms);
//}

static void midi_note_on(uint16_t note, uint8_t vel, uint8_t chan){
	if(vel > 0){
		MIDI.sendNoteOn(note, vel, chan);
	} 
	else {
		MIDI.sendNoteOff(note, vel, chan);
	}
}

//void init_midi_seq(sequencer* s){
//	track* t;
//	s->set_current_track(0);
//	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
//		t = s->get_track(i);
//		t->set_out_id(i+1);
//		t->set_all_step_note(MIDI_DRUM_GM[i]);
//		t->init_hw_clbk(midi_note_on);
//		
//	}
//}

// TEMPORARY
//void init_midi_controller(test_proj_one* p){
//	p->init_hw_clbk(midi_note_on);
//}

void init_midi(){
	MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.turnThruOff();
	
//	midi_sync.clk_set_operation(MIDI_SYNC_PPN,midi_sync.clk_get_ms());
	midi_sync.clk_set_ratio(midi_sync.clk_get_ms(), 1, MIDI_SYNC_PPN);
//	init_clk(&midi_sync);	
}

void midi_loop(boolean flg){
//	MIDI.read();
//	if(flg){
//		MIDI.sendRealTime(MIDI_NAMESPACE::Clock);
//	}
}

	
