/*

 Circuit Abbey EuroDuino
 Arduino EuroRack Module
 
 sigan: tick - burst generator
 
 V0.00

 Vincent deadpix Dupre
 
*/
 
#include <clock.h>
//#include <burst.h>

#define NR_CLOCK		2
#define NR_BURSTED_TICKS	2


// Default Const used from Circuit Abbey Dual LFO Sketch - Tank
// These constants won't change.
// They're used to give names to the pins used
const int ain1 = A0;	// Analog Input 1
const int ain2 = A1;   	// Analog Input 2
const int pot1  = A2;   // Pot 1
const int pot2  = A3;   // Pot 2
const int din1  = 8;    // Digital Input 1
const int din2  = 9;    // Digital Input 2
const int dout1 = 3;   	// Digital Output 1
const int dout2 = 4;   	// Digital Output 2
const int aout1 = 5;    // Analog Output 1 
const int aout2 = 6;    // Analog Output 2 
const int sw1up = A4;   // Switch 1 Up
const int sw1dw = A5;   // Switch 1 Dwn
const int sw2up = 7;    // Switch 2 Up
const int sw2dw = 2;    // Switch 2 Dwn


/* contains dout1, dout2, etc in function of the number of clock output */
//const int dout_clk[NR_CLOCK] = {dout1,dout2};
//const int dout_burst[NR_BURSTED_TICKS] = {aout1,aout2};

/* reference for burst placement if needed */
//byte ref_burst_placement_asc[NR_MAX_BURST];
//byte ref_burst_placement_des[NR_MAX_BURST];
//byte arr_burst_placement[NR_MAX_BURST];

/* Global variables */
volatile unsigned char din_state;
volatile unsigned char sw1_state;
volatile unsigned char sw2_state;

#define SW_UP				0x1
#define SW_MID				0x3
#define SW_DOWN				0x2

#define MIN_DIVIDER  		2
#define MAX_DIVIDER 		16


/* Global variable */
//clock c;
//burst b[NR_BURSTED_TICKS];
//int burst_flag[NR_BURSTED_TICKS];
//velocity v;

ISR (PCINT0_vect){
	din_state = ( (digitalRead(din1) << 0) | (digitalRead(din2) << 1) );
}

ISR (PCINT1_vect){
	sw1_state = ( (digitalRead(sw1up) << 1) | (digitalRead(sw1dw) << 0) );
}

ISR (PCINT2_vect){
	sw2_state = ( (digitalRead(sw2up) << 1) | (digitalRead(sw2dw) << 0) );
}

int get_pot1(){
	return analogRead(pot1);
}
int get_pot2(){
	return analogRead(pot2);
}

/*
void InitializeVar(){
	int init_bpm= 90;
	int temp1[] = {45};
	int temp2[] = {50};
	int temp3[] = {2};
	din_state 	= 0;
	sw1_state 	= ( (digitalRead(sw1up) << 1) | (digitalRead(sw1dw) << 0) );
	sw2_state 	= ( (digitalRead(sw2up) << 1) | (digitalRead(sw2dw) << 0) );
//	clk.initialize(init_bpm,50);
	c.init_master(init_bpm,50);
	c.init_slaves(temp1,temp2,1);
	c.set_slaves_divider(temp3);
	for(int i=0;i<NR_BURSTED_TICKS;i++){
		b[i].init_burst(8, 4, init_bpm, 50);
	}
	v.init_velocity(1000,1000);
}
*/
void init_io(){
        /* Set up I/O pins */
        pinMode(din1,  INPUT);
        pinMode(din2,  INPUT);
	pinMode(ain1,  INPUT);
	pinMode(ain2,  INPUT);
        pinMode(dout1, OUTPUT);
        pinMode(dout2, OUTPUT);
        pinMode(aout1, OUTPUT);
        pinMode(aout2, OUTPUT);
        pinMode(sw1up, INPUT_PULLUP);
        pinMode(sw1dw, INPUT_PULLUP);
        pinMode(sw2up, INPUT_PULLUP);
        pinMode(sw2dw, INPUT_PULLUP);
}

void init_interrupt(){
	cli(); // switch interrupts off while messing with their settings

	PCICR  |= (1 << PCIE0);   // set PCIE0 to enable PCMSK0 scan
	PCICR  |= (1 << PCIE1);   // set PCIE1 to enable PCMSK1 scan
	PCICR  |= (1 << PCIE2);   // set PCIE2 to enable PCMSK2 scan

	PCMSK0 |= (1 << PCINT0);  // set PCINT0 to trigger an irq on state change
	PCMSK0 |= (1 << PCINT1);  // set PCINT1 to trigger an irq on state change

	PCMSK1 |= (1 << PCINT12); // set PCINT12 to trigger an irq on state change
	PCMSK1 |= (1 << PCINT13); // set PCINT13 to trigger an irq on state change
	
	PCMSK2 |= (1 << PCINT18); // set PCINT18 to trigger an irq on state change
	PCMSK2 |= (1 << PCINT23); // set PCINT23 to trigger an irq on state change

	sei(); // turn on interrupts
}

void init_random(){
	randomSeed(analogRead(0));
}


void setup() {
	Serial.begin(9600);
	Serial.println("Begin");

	init_io();
	init_interrupt();
	init_random();
//	init_var();
}

int bank_time(int sw_state){
	if(sw_state == SW_UP){
		/* save clock interval and gate len */
		int p1 = get_pot1();
		int p2 = get_pot2();
//		c.bank_master_clock_bpm(p1);
//		c.bank_master_clock_len(p2);		
//		c.bank_slaves_clock_bpm(p1);
//		c.bank_slaves_clock_len(p2);
		/* Have to update also bursted ticks */
//		for(int i=0; i<NR_BURSTED_TICKS; i++){
//			b[i].bank_burst_bpm(p1);
//			b[i].bank_burst_tick_len(p2);
//		}
		return 1;
	} 
	else if(sw_state == SW_DOWN){
//		clk.set_rand_tick_len(get_pot1());
		int p1 = get_pot1();
//		c.bank_master_random_clock_len(p1);
//		c.bank_slaves_random_clock_len(p1);
//		c.bank_slave_divider(get_pot2(), 0);
//		/* update bursted ticks with random tick/gate len */
//		for(int i=0; i<NR_BURSTED_TICKS; i++){
//			b[i].bank_burst_random_clock_len(p1);
//		}
		return 0;
	} 
}

void bank_burst(int sw_state){
	int p1 = get_pot1();
	int p2 = get_pot2();
	if(sw_state == SW_UP){
//		for(int i=0; i<NR_BURSTED_TICKS; i++){
//			b[i].bank_burst_resolution(p1);
//			b[i].bank_nr_burst(p2);
//		}
	} else if(sw_state == SW_DOWN){
//		for(int i=0; i<NR_BURSTED_TICKS; i++){
//			// random burst placement
//		}		
	}
}

void bank_random(int sw_state){
	int p1 = get_pot1();
	int p2 = get_pot2();

	if(sw_state == SW_UP){
		/* bank something */
	} 
	else if(sw_state == SW_DOWN){
		/* bank velocity */
//		Serial.print("bank vel ");
	//	Serial.print(p1);
		//Serial.print(" ");
		//Serial.println(p2);
//		v.bank_velocity_lim(p1,p2);
	}
}



int bank_all(){
	int sync_mode = 0;
	/* Save the pot values if not in lock position */
	if(sw2_state != SW_MID){
		switch (sw1_state) {
			case SW_UP:
				/* Time */
				sync_mode = bank_time(sw2_state);
				break;
				
			case SW_MID:
				/* burst */
				bank_burst(sw2_state);
				break;
	
			case SW_DOWN:
				/* random */
				bank_random(sw2_state);
				break;
		}
	}
	return sync_mode;
}

void update_dout_from_clk_state(const int out, int state){
	if(state == 1){
		digitalWrite(out,HIGH);
	}
	else if(state == 0){
		digitalWrite(out,LOW);		
	}
}

void update_clk(int* status){
	for(int i=0;i<NR_CLOCK;i++){
		update_dout_from_clk_state(dout_clk[i], status[i]);
	}
}	

void update_burst(int state, int idx){
	update_dout_from_clk_state(dout_burst[idx], state);
}

void write_analog(const int aout, int val){
	analogWrite(aout, (255-val));
}

void update_velocity(int state, int idx){
	if(state == 1){
		int vel = v.get_velocity();
//		Serial.print("vel=");
//		Serial.println(vel);
		write_analog(aout_velocity[idx], vel);
	} 
	else if(state == 0){
		write_analog(aout_velocity[idx], 0);
	}
	
}

void loop(){
	int state[NR_CLOCK];
	int sync_mode = bank_all();
	
	c.get_clock_status(state,1);
	update_clk(state);
	
	/* update the state of bursted tick when the master clock has been fired	*/
	/* state of bursted ticks are ONLY updated on new master clock 				*/
	if(state[0] == 1){
		/* check the state of the digital input for the burst */
		for(int i=0;i<NR_BURSTED_TICKS;i++){
			burst_flag[i] = (din_state >> i) & 0x1;
		}
	}
	
	/* Check bursted tick: 								*/
	/*  - synchronize with the master clock (state[0]) 	*/
	/* 	- generate a burst if the digital input is high	*/
	for(int i=0;i<NR_BURSTED_TICKS;i++){
		if(burst_flag[i] == 1){
			int burst_state;
			/* check whether we need to resync with master clock (state[0]) */
			if(state[0] == 1){
				/*TODO replace millis() with master sync time */
				//b[i].reset_burst(millis());
				b[i].reset_burst(c.get_master_sync_time());
				burst_state = 1;
			}
			else {
				burst_state = b[i].get_burst_status();
//				if(state[0] == 1) Serial.println("ouaich");
			}
			update_burst(burst_state,i);
			update_velocity(burst_state,i);
		} else {
			update_velocity(0,i);
		}
	}
	
	/* we did a good job, let's rest for 1ms */
	delay(1);
}
