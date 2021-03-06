#ifndef __MIDI_CST_H__
#define __MIDI_CST_H__

static const uint8_t MIDI_DRUM_GM[NR_LEDS] = { 
	37, 36, 42, 82, 
	40, 38, 46, 44, 
	48, 47, 45, 49, 
	43, 55, 51, 53,

	54, 69, 81, 80, 
	65, 66, 76, 77, 
	56, 62, 63, 64, 
	73, 74, 71, 39,
	
	52, 57, 58, 59, 
	60, 61, 67, 68, 
	70, 72, 75, 78, 
	79, 35, 41, 50,
	
	83, 84, 85, 86, 
	87, 88, 89, 90, 
	91, 92, 93, 94, 
	95, 96, 97, 98,
};

static const uint8_t MPC1000_PAD[4][4][4] = {
	{
		{43, 55, 51, 53},
		{48, 47, 45, 49},
		{40, 38, 46, 44},
		{37, 36, 42, 82},
	},
	{
		{73, 74, 71, 39},
		{65, 66, 76, 77},
		{56, 62, 63, 64},
		{54, 69, 81, 80},
	},
	{
		{79, 35, 41, 50},
		{70, 72, 75, 78},
		{60, 61, 67, 68},
		{52, 57, 58, 59},
	},
	{
		{95, 96, 97, 98},
		{91, 92, 93, 94},
		{87, 88, 89, 90},
		{83, 84, 85, 86},
	},
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

#define MIDI_C1_OFFSET	mC1

#endif
