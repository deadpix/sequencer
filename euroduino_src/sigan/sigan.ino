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

#define EXT_MASTER_CLK		(1<<0)
#define EXT_SLAVE_CLK		(1<<1)

#define MIN_ANALOG_OUT		0
#define MAX_ANALOG_OUT		255
#define MAX_ANALOG_IN		1022

const int clk_rate[] = {-8, -7, -6, -5, -4, -3, -2, 1, 2, 3, 4, 5, 6, 7, 8};
const int MAX_CLK_SLAVE_RATE = sizeof(clk_rate) / sizeof(clk_rate[0]);


/* Global variable */
//clock c;
//burst b[NR_BURSTED_TICKS];
//int burst_flag[NR_BURSTED_TICKS];
//velocity v;

clk master;
clk slave;
clk rnd_clk[2];
int max_rnd[2];


gate m_gate;
gate s_gate;

bool sync_master;
clk  clk_burst[NR_OUT];
//gate s_gate[NR_OUT];
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
}

ISR (PCINT2_vect){
	sw2_state = ( (digitalRead(sw2up) << 1) | (digitalRead(sw2dw) << 0) );
}

static void init_rnd(){
	max_rnd[0] = MAX_ANALOG_OUT + 1;
	max_rnd[1] = MAX_ANALOG_OUT + 1;
}

static void wr_gate_out(int out, bool val){
	if(val)
		digitalWrite(out,HIGH);
	else
		digitalWrite(out,LOW);	
}

static void upd_rev_out(){
	digitalWrite( dout2, !(digitalRead(din2)) );
}



static unsigned int ckeck_ext_clk(){
//	unsigned int tmp;
	unsigned int ms = 0; 
	if(din_state & EXT_MASTER_CLK){
		ms = ext_clk_period;
		ext_clk_period = 0;		
		din_state &= ~(EXT_MASTER_CLK);
	
		Serial.print("ms ");
		Serial.println(ms);
	
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

static unsigned int get_pot_x(int x){
	unsigned int val = analogRead(x);
	val &= !(0x1);
	return val;
}

static unsigned int get_max_rnd_val(unsigned int in){
	unsigned int ret = map(in, 0, MAX_ANALOG_IN, MIN_ANALOG_OUT, MAX_ANALOG_OUT);
	return ret;
}

static int get_rnd_clk(unsigned int in){
	int idx = map(in, 0, MAX_ANALOG_IN, 0, (MAX_CLK_SLAVE_RATE-1));
	return clk_rate[idx];	
}

void init_var(){
	ext_clk_flag = false;
	ext_slave_flag = false;
	sync_master = false;
	m_gate.set_hw_cbck(dout1, wr_gate_out);
	s_gate.set_hw_cbck(dout2, wr_gate_out);
//	s_gate[0].set_hw_cbck(dout1, wr_gate_out);
//	s_gate[1].set_hw_cbck(dout2, wr_gate_out);
	sw1_state = ( (digitalRead(sw1up) << 1) | (digitalRead(sw1dw) << 0) );
	sw2_state = ( (digitalRead(sw2up) << 1) | (digitalRead(sw2dw) << 0) );
}

void init_io(){
	/* Set up I/O pins */
	pinMode(din1,  INPUT);
	pinMode(din2,  INPUT);
	pinMode(ain1,  INPUT);
	pinMode(ain2,  INPUT);
	pinMode(dout1, OUTPUT);
	pinMode(dout2, OUTPUT);
//	pinMode(aout1, OUTPUT);
//	pinMode(aout2, OUTPUT);
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
	init_rnd();
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
	int p1 = get_rnd_clk(get_pot1() & ~(0x1));
	unsigned int p2 = get_max_rnd_val(get_pot2() & ~(0x1));

	if(sw_state == SW_UP){
		/* bank something */
		rnd_clk[0].clk_set_operation(p1, master.clk_get_ms());
		max_rnd[0] = p2 + 1;			
	}
	else if(sw_state == SW_DOWN){
		rnd_clk[1].clk_set_operation(p1, master.clk_get_ms());
		max_rnd[1] = p2 + 1;			
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
//				bank_burst(sw2_state);
				break;
	
			case SW_DOWN:
				/* random */
				bank_random(sw2_state);
				break;
		}
	}
	return sync_mode;
}

/*
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
*/

void upd_output(unsigned int master_ms){
	if(master_ms > 0){
		m_gate.rst_gate(true);
	}
	else {
		m_gate.upd_gate();
	}
}

static void upd_rnd_output1(){
	byte rnd = random(max_rnd[0]);
//	Serial.println(rnd);
//	analogWrite(aout1, rnd);
	analogWrite(aout1, (255-rnd));
}

static void upd_rnd_output2(){
	byte rnd = random(max_rnd[1]);
//	analogWrite(aout2, (255 - rnd));
	analogWrite(aout2, rnd);
}

void loop(){
	uint32_t rnd_ms[2];
	uint16_t step = master.clk_get_step_cnt();
	unsigned int ms = ckeck_ext_clk();
//	unsigned int ms_slave = ckeck_ext_slave_clk();
	int sync_mode = bank_all(ms);

	if(!ext_clk_flag){
		ms = master.clk_elapsed();
	}

//	/* copy state of trig input at each new clock */
	if(ms > 0){
		// sync random clock
		rnd_ms[0] = rnd_clk[0].clk_sync(ms, step);
		rnd_ms[1] = rnd_clk[1].clk_sync(ms, step);
	}
	else {
		rnd_ms[0] = rnd_clk[0].clk_elapsed();
		rnd_ms[1] = rnd_clk[1].clk_elapsed();
	}
//		trig = din_state;

	upd_output(ms);
	upd_rev_out();

	if(rnd_ms[0] > 0){
		upd_rnd_output1();
	}
	if(rnd_ms[1] > 0){
//		Serial.println("upd rnd 2");
		upd_rnd_output2();
	}

//	analogWrite(aout1, 127);
	/* we did a good job, let's rest for 1ms */
	delay(5);
}
