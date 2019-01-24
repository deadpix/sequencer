#include <SoftwareSerial.h>
#include <MIDI.h>
#include "src/clk.h"
#include "src/sequencer.h"
#include "src/track.h"
#include "src/test_proj_one.h"
#include "src/hw.h"

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

static const uint8_t MIDI_DRUM_GM[NR_LEDS] = { 
	37, 36, 42, 82, 40, 38, 46, 44, 48, 47, 45, 49, 43, 55, 51, 53,
	54, 69, 81, 80, 65, 66, 76, 77, 56, 62, 63, 64, 73, 74, 71, 39,
	52, 57, 58, 59, 60, 61, 67, 68, 70, 72, 75, 78, 79, 35, 41, 50,
	83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
};

enum MIDI_KEY {
	mA0=21,mA0s=22,mB0f=22,mB0=23, 
	mC1=24,mC1s=25,mD1f=25,mD1=26,mD1s=27,mE1f=27,mE1=28,mF1=29,mF1s=30,mG1f=30,mG1=31,mG1s=32,mA1f=32,mA1=33,mA1s=34,mB1f=34,mB1=35,
	mC2=36,mC2s=37,mD2f=37,mD2=38,mD2s=39,mE2f=39,mE2=40,mF2=41,mF2s=42,mG2f=42,mG2=43,mG2s=44,mA2f=44,mA2=45,mA2s=46,mB2f=46,mB2=47,
	mC3=48,mC3s=49,mD3f=49,mD3=50,mD3s=51,mE3f=51,mE3=52,mF3=53,mF3s=54,mG3f=54,mG3=55,mG3s=56,mA3f=56,mA3=57,mA3s=58,mB3f=58,mB3=59,
	mC4=60,mC4s=61,mD4f=61,mD4=62,mD4s=63,mE4f=63,mE4=64,mF4=65,mF4s=66,mG4f=66,mG4=67,mG4s=68,mA4f=68,mA4=69,mA4s=70,mB4f=70,mB4=71,
	mC5=72,mC5s=73,mD5f=73,mD5=74,mD5s=75,mE5f=75,mE5=76,mF5=77,mF5s=78,mG5f=78,mG5=79,mG5s=80,mA5f=80,mA5=81,mA5s=82,mB5f=82,mB5=83,
	mC6=84,mC6s=85,mD6f=85,mD6=86,mD6s=87,mE6f=87,mE6=88,mF6=89,mF6s=90,mG6f=90,mG6=91,mG6s=92,mA6f=92,mA6=93,mA6s=94,mB6f=94,mB6=95,
	mC7=96,mC7s=97,mD7f=97,mD7=98,mD7s=99,mE7f=99,mE7=100,mF7=101,mF7s=102,mG7f=102,mG7=103,mG7s=104,mA7f=104,mA7=105,mA7s=106,mB7f=106,mB7=107,
	mC8=108
};

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
	MIDI.read();
	if(flg){
		MIDI.sendRealTime(MIDI_NAMESPACE::Clock);
	}
}

	
