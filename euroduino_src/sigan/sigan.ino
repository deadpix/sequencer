/*

 Circuit Abbey EuroDuino
 Arduino EuroRack Module
 
 sigan: tick - burst generator
 
 V0.00

 Vincent deadpix Dupre
 
*/
 
//#include <clock.h>
//#include <burst.h>
#include <elapsedMillis.h>

#include "gate.h"
#include "clk.h"

#define NR_CLOCK		2
#define NR_BURSTED_TICKS	2

#define MIN_BPM			40
#define MAX_BPM			300

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
volatile unsigned char ain_state;

#define SW_UP				0x1
#define SW_MID				0x3
#define SW_DOWN				0x2

#define MIN_DIVIDER  		2
#define MAX_DIVIDER 		16

#define NR_OUT				2
#define MAX_CLK_PERIOD		(40*60*1000)

#define EXT_MASTER_CLK		(0<<1)
#define EXT_SLAVE_CLK		(1<<1)

/* Global variable */
//clock c;
//burst b[NR_BURSTED_TICKS];
//int burst_flag[NR_BURSTED_TICKS];
//velocity v;

clk master;
clk slave;

gate m_gate;
gate s_gate;

bool sync_master;
clk  clk_burst[NR_OUT];
gate s_gate[NR_OUT];
byte trig;

bool ext_clk_flag;
bool ext_slave_flag;
elapsedMillis ext_clk_period;
elapsedMillis ext_clk_slave_period;

ISR (PCINT0_vect){
	din_state = ( (digitalRead(din1) << 0) | (digitalRead(din2) << 1) );
}

ISR (PCINT1_vect){
	sw1_state = ( (digitalRead(sw1up) << 1) | (digitalRead(sw1dw) << 0) );
	sw2_state = ( (digitalRead(sw2up) << 1) | (digitalRead(sw2dw) << 0) );
}

ISR (PCINT2_vect){
	ain_state = (digitalRead(ain1) << 0) | (digitalRead(ain2) << 1);
}

static void wr_gate_out(int out, bool val){
	if(val)
		digitalWrite(out,HIGH);
	else
		digitalWrite(out,LOW);	
}

static void upd_rev_out(){
	digitalWrite( aout2, !(digitalRead(ain2)) );
}



static unsigned int ckeck_ext_clk(){
//	unsigned int tmp;
	unsigned int ms = 0; 
	if(ain_state & EXT_MASTER_CLK){
		ms = ext_clk_period;
		ext_clk_period = 0;		
		ain_state &= ~(EXT_MASTER_CLK);
		
		if(ext_clk_flag){
			if(ms > MAX_CLK_PERIOD){
				ms = 0;
				ext_clk_flag = false;
			} 
			else {
				ext_clk_flag = true;
			}
		} 
		else {
			ms = 0;
			ext_clk_flag = true;
		}	
	}
	return ms;
}

static unsigned int ckeck_ext_slave_clk(){
	unsigned int ms = 0; 
	if(ain_state & EXT_SLAVE_CLK){
		ms = ext_clk_slave_period;
		ext_clk_slave_period = 0;
		ain_state &= ~(EXT_SLAVE_CLK);

		if(ext_slave_flag){
			if(ms > MAX_CLK_PERIOD){
				ms = 0;
				ext_slave_flag = false;
			}
			else {
				ext_slave_flag = true;
			}
		}
		else {
			ms = 0;
			ext_slave_flag = true;
		}
	}
	return ms;
}




int get_pot1(){
	return analogRead(pot1);
}
int get_pot2(){
	return analogRead(pot2);
}

void init_var(){
	ext_clk_flag = false;
	ext_slave_flag = false;
	sync_master = false;
	m_gate.set_hw_cbck(dout1, wr_gate_out);
	s_gate.set_hw_cbck(dout2, wr_gate_out);
//	s_gate[0].set_hw_cbck(dout1, wr_gate_out);
//	s_gate[1].set_hw_cbck(dout2, wr_gate_out);
}

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
/*
	PCMSK0 |= (1 << PCINT0);  // set PCINT0 to trigger an irq on state change
	PCMSK0 |= (1 << PCINT1);  // set PCINT1 to trigger an irq on state change

	PCMSK1 |= (1 << PCINT12); // set PCINT12 to trigger an irq on state change
	PCMSK1 |= (1 << PCINT13); // set PCINT13 to trigger an irq on state change
	
	PCMSK2 |= (1 << PCINT18); // set PCINT18 to trigger an irq on state change
	PCMSK2 |= (1 << PCINT23); // set PCINT23 to trigger an irq on state change
*/
	PCMSK0 |= (1 << PCINT0);  // set PCINT0 to trigger an irq on state change
	PCMSK0 |= (1 << PCINT1);  // set PCINT1 to trigger an irq on state change

	PCMSK1 |= (1 << PCINT12); // set PCINT12 to trigger an irq on state change
	PCMSK1 |= (1 << PCINT13); // set PCINT13 to trigger an irq on state change
	PCMSK2 |= (1 << PCINT18); // set PCINT18 to trigger an irq on state change
	PCMSK2 |= (1 << PCINT23); // set PCINT23 to trigger an irq on state change

//	PCMSK2 |= (1 << PCINT8);
//	PCMSK2 |= (1 << PCINT9);

	sei(); // turn on interrupts
}

void init_random(){
	randomSeed(analogRead(0));
}

static void upd_gate_len(gate* g, clk* c, int percent){
	unsigned int ms = c->clk_get_ms() * percent / 100;
	g->set_gate_len(ms);
}

void setup() {
	Serial.begin(9600);
	Serial.println("Begin");

	init_io();
	init_interrupt();
	init_random();
	init_var();
}

int check_clk(){
	

}

int bank_time(int sw_state, unsigned int ms_period){
	int ret;
	if(sw_state == SW_UP){
		/* save clock interval and gate len */
		int p1 = get_pot1();
		int p2 = get_pot2();
		int tmp;

		if(ext_clk_flag){
			tmp = map(p1, 0, 1023, 0, 48) * 2;	
			sync_master = master.clk_set_ms( (ms_period / tmp) );
			if(!ext_slave_flag)
				slave.clk_sync_intern((ms_period / tmp));
		}
		else {
			tmp = map(p1, 0, 1023, MIN_BPM, MAX_BPM);
			sync_master = master.clk_set_bpm(tmp);
			if(!ext_slave_flag)
				slave.clk_sync_intern(master.clk_get_ms());
		}

		tmp = map(p2, 0, 1023, 0, 99);
		upd_gate_len(&m_gate, &master, tmp);
		upd_gate_len(&s_gate, &slave, tmp);

		//TODO sync / update burst

		return 1;
	} 
	else if(sw_state == SW_DOWN){
		int p1 = get_pot1();
		return 0;
	} 
}

void bank_burst(int sw_state){
	int p1 = get_pot1();
	int p2 = get_pot2();

	if(sw_state == SW_UP){
	
	}
	else if(sw_state == SW_DOWN){
	
	}
}

void bank_random(int sw_state){
	int p1 = get_pot1();
	int p2 = get_pot2();

	if(sw_state == SW_UP){
		/* bank something */
	}
	else if(sw_state == SW_DOWN){
	
	}
}



int bank_all(unsigned int ms){
	int sync_mode = 0;
	/* Save the pot values if not in lock position */
	if(sw2_state != SW_MID){
		switch (sw1_state) {
			case SW_UP:
				/* Time */
				sync_mode = bank_time(sw2_state,ms);
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

void upd_output(unsigned int master_ms, unsigned int slave_ms){
	if(ext_slave_flag){
		if(slave_ms > 0){
			s_gate.rst_gate(true);
		} 
		else {
			s_gate.upd_gate();
		}
	}
	

	if(master_ms > 0){
		m_gate.rst_gate(true);
		if(!ext_slave_flag){
			// resync slave

		}
	}
	else {
		m_gate.upd_gate();
	}
}



void loop(){
	unsigned int ms = ckeck_ext_clk();
	unsigned int ms_slave = ckeck_ext_slave_clk();
	int sync_mode = bank_all(ms);

	if(!ext_clk_flag){
		ms = master.clk_elapsed();
	}

	/* copy state of trig input at each new clock */
	if(ms > 0)
		trig = din_state;

	upd_output(ms, ms_slave);

	/* we did a good job, let's rest for 1ms */
	delay(1);
}
