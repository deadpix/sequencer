/*
 *	Copyright (c) 2017 Vincent "deadpix" Dupre
 *
 *	Author: Vincent Dupre (vdupre@gmail.com)
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */ 

/*
 *  Circuit Abbey EuroDuino
 *  Arduino EuroRack Module
 *
 *  sigan: 
 *	- gates generator with variable gate length (digital out)
 *	- random CV generator synced with gate generator (analog out)
 *	- syncable via digital input with rising / falling sync configurable
 *
 *  V1.00
 */
 
#include <elapsedMillis.h>
#include "gate.h"
#include "clk.h"

#define MIN_BPM				40
#define MAX_BPM				300
#define SW_UP				0x1
#define SW_MID				0x3
#define SW_DOWN				0x2

#define MAX_CLK_PERIOD		(1500)
#define EXT_MASTER_CLK		(1<<0)
//#define EXT_SLAVE_CLK		(1<<1)

#define MIN_ANALOG_OUT		0
#define MAX_ANALOG_OUT		255
#define MAX_ANALOG_IN		1023

#define NR_ANALOG_OUTPUT	2

#define RANDOM_CV			3
#define PWM_CV				1
#define SLV_CV				2


const int ain1  = A0;	// Analog Input 1
const int ain2  = A1;  	// Analog Input 2
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

/* Global variables for interrupt service routine */
volatile unsigned char din_state;
volatile unsigned char sw1_state;
volatile unsigned char sw2_state;

/* Global variable */
const int clk_rate[] = {-8, -7, -6, -5, -4, -3, -2, 1, 2, 3, 4, 5, 6, 7, 8};
const int MAX_CLK_SLAVE_RATE = sizeof(clk_rate) / sizeof(clk_rate[0]);

struct gated_clock_t {
	/* class */
	clk c;				// clock data structure
	gate g;				// control pw of clock
	/* data to be save (pot or cv) */
	byte clk_trig_lvl;	// rising or falling edge
	byte clk_gate_len;	// express as a percentage
	int  clk_rate;		// base clock div/mult
};

struct gated_clock_t mst_clk;
struct gated_clock_t slv_clk;

clk master;
clk slave;
gate m_gate;
gate s_gate;
byte percent_gate_len[2];
byte slv_trig_level;
byte ext_trig_level;
int master_rate;
byte slv_mult;


/************************** RANDOM **************************/
struct rnd_t {
	clk rnd_clk;
	int rnd_max;
	int rnd_pot;
};
struct rnd_t rnds[NR_ANALOG_OUTPUT];
/************************************************************/



bool sync_master;
//elapsedMillis ext_clk_slave_period;


/***************** DIGITAL INPUT SYNC ***********************/
// save state of digital input (HIGH or LOW)
// so that ckeck_..._trig(trig_lvl) can determine 
// rising or fallig edge of digital input incoming data 
// TODO: might be simpler with IRQ
// save status of digital input 1
byte ext_trig_status;
// save status of digital input 2
byte slv_trig_status;
/************************************************************/


/******* EXTERNAL CLOCK connected via digital input 1 *******/
// data structure used for determining if mst_clk is driven
// externally (via digital input 1) or internally
struct ext_clock_t {
	int ext_state;	// count number of ext clock trig => used for sync
	int ext_ms;		// store ext clock period in ms
};
ext_clock_t ext_clk;
// store mst_clk feeding
// 	true:  mst_clk feed externally 
// 	false: mst_clk feed internally
bool ext_clk_flag;
// calculate time between two digital input 1 trig
elapsedMillis ext_clk_period;

/* ext clk: old code start */
int ext_clk_state;
unsigned int ext_clk_ms;
/* ext clk: old code end */
/************************************************************/



ISR (PCINT0_vect){
//	din_state = ( (digitalRead(din1) << 0) | (digitalRead(din2) << 1) );
}

ISR (PCINT1_vect){
	sw1_state = ( (digitalRead(sw1up) << 1) | (digitalRead(sw1dw) << 0) );
}

ISR (PCINT2_vect){
	sw2_state = ( (digitalRead(sw2up) << 1) | (digitalRead(sw2dw) << 0) );
}



static byte wr_gate_out(int out, bool val){
	if(val)
		digitalWrite(out,HIGH);
	else
		digitalWrite(out,LOW);	
}

static unsigned int get_pot1(){
	return analogRead(pot1);
}
static unsigned int get_pot2(){
	return analogRead(pot2);
}
static unsigned int get_ain1(){
	return analogRead(ain1);
}
static unsigned int get_ain2(){
	return analogRead(ain2);
}

// uin32_t ==> 32bit positive integer
// unsigned int ==> 16bit positive integer >= 0!!!
static unsigned int attenuate_input(unsigned int in, unsigned int attenuator){
	uint32_t tmp = (uint32_t) in * (uint32_t) attenuator / MAX_ANALOG_IN;
	return (unsigned int) tmp;
}

/* 
 * poll digital input 1: 
 * 	==> previous read saved in ext_trig_status 
 */
static bool ckeck_ext_trig(byte trig_lvl){
	bool ret = false;

	byte curr_status = digitalRead(din1);
	if(curr_status == trig_lvl){
		if(curr_status != ext_trig_status){
			ret = true;
		}
	}
	ext_trig_status = curr_status;
	return ret;
}

/* 
 * poll digital input 2: 
 * 	==> previous read saved in slv_trig_status 
 */
static bool check_slv_trig(byte trig_lvl){
	bool ret = false;
	byte curr_status = digitalRead(din2);
	if(curr_status == trig_lvl){
		if(curr_status != slv_trig_status){
			ret = true;
		}
	}
	slv_trig_status = curr_status;
	return ret;
}

static unsigned int ckeck_ext_clk(){
	unsigned int ms = 0; 
	if(ckeck_ext_trig(ext_trig_level)){
		ms = ext_clk_period;
		ext_clk_period = 0;		
		din_state &= ~(EXT_MASTER_CLK);

		/* ext clk: new code start */
		if(ext_clk.ext_state == 0){
			ext_clk.ext_state = 1;
			ms = 0;
		}
		else {
			ext_clk_flag = true;
			ext_clk.ext_state = (ext_clk.ext_state % master_rate)+1;
			// need to resync master	
			ext_clk.ext_ms = ms;
		}
		/* ext clk: new code end */


		/* ext clk: old code start */	
//		if(ext_clk_state == 0){
//			ext_clk_state = 1;
//			ms = 0;
//		}
//		else {
//			ext_clk_flag = true;
//			ext_clk_state = (ext_clk_state % master_rate)+1;
//			// need to resync master	
//			ext_clk_ms = ms;
//		}
		/* ext clk: old code end */	
		
	} 

	/* ext clk: new code start */
	else if(ext_clk.ext_state){
		if(ext_clk_period > MAX_CLK_PERIOD){
			ext_clk.ext_ms = 0;
			ms = 0;
			ext_clk.ext_state = 0;
			ext_clk_flag = false;
		}
	}
	/* ext clk: new code end */

	/* ext clk: old code start */	
//	else if(ext_clk_state){
//		if(ext_clk_period > MAX_CLK_PERIOD){
//			ext_clk_ms = 0;
//			ms = 0;
//			ext_clk_state = 0;
//			ext_clk_flag = false;
//		}
//	}
	/* ext clk: old code end */	

	return ms;
}
static int get_clk_rate(unsigned int in){
	int idx = map(in, 0, MAX_ANALOG_IN, 0, (MAX_CLK_SLAVE_RATE-1));
	return clk_rate[idx];	
}

void init_ext_clk(){
	ext_clk_flag = false;
	ext_clk.ext_state = 0;
	ext_clk.ext_ms = 0;
}

void init_var(){
//	ext_clk_flag = false;
//	
//	/* ext clk: old code start */
//	ext_clk.ext_state = 0;
	/* ext clk: old code end */
	/* ext clk: old code start */
//	ext_clk_state = 0;
	/* ext clk: old code end */

	sync_master = false;
	m_gate.set_hw_cbck(dout1, wr_gate_out);
	s_gate.set_hw_cbck(dout2, wr_gate_out);
	sw1_state = ( (digitalRead(sw1up) << 1) | (digitalRead(sw1dw) << 0) );
	sw2_state = ( (digitalRead(sw2up) << 1) | (digitalRead(sw2dw) << 0) );
	master_rate = 1;
//	ext_clk_ms = 0;

	slv_trig_level = HIGH;
	ext_trig_level = HIGH;

	percent_gate_len[0] = 50;
	percent_gate_len[1] = 50;
	upd_gate_len(&m_gate, &master, percent_gate_len[0]);
	upd_gate_len(&s_gate, &slave, percent_gate_len[1]);
}

void init_io(){
	/* Set up I/O pins */
	pinMode(din1,  INPUT);
	pinMode(din2,  INPUT);
	pinMode(ain1,  INPUT);
	pinMode(ain2,  INPUT);
	pinMode(dout1, OUTPUT);
	pinMode(dout2, OUTPUT);
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

static void upd_gate_len(gate* g, clk* c, int percent){
	unsigned int ms = c->clk_get_ms() * percent / 100;
	g->set_gate_len(ms);
}

int bank_time(int sw_state, unsigned int ms_period){
	int ret = 0;
	if(sw_state == SW_UP){
		/* save clock interval and gate len */
		int p1 = get_pot1();
		int p2 = get_pot2();
		int tmp;

		if(ext_clk_flag){
			tmp = get_clk_rate(p1);

			if(tmp < 0)
				master_rate = abs(tmp);
			else
				master_rate = 1;
		
			if(!master.clk_set_operation(tmp, ext_clk.ext_ms))
				Serial.println("error");
			
		}
		else {
			tmp = map(p1, 0, 1023, MIN_BPM, MAX_BPM);
			sync_master = master.clk_set_bpm(tmp);
		}

		int idx = map(get_pot2(), 0, MAX_ANALOG_IN, 7, (MAX_CLK_SLAVE_RATE-1));
		
		slv_mult = clk_rate[idx];
	  	slave.clk_set_operation(clk_rate[idx],master.clk_get_ms());
		
		rnds[0].rnd_clk.clk_sync_intern(master.clk_get_ms());
		rnds[1].rnd_clk.clk_sync_intern(master.clk_get_ms());

		upd_gate_len(&m_gate, &master, percent_gate_len[0]);
		upd_gate_len(&s_gate, &slave, percent_gate_len[1]);

		//TODO sync / update burst
	} 
	else if(sw_state == SW_DOWN){
		percent_gate_len[0] = map(get_pot1(), 0, 1023, 0, 99);
		percent_gate_len[1] = map(get_pot2(), 0, 1023, 0, 99);
		upd_gate_len(&m_gate, &master, percent_gate_len[0]);
		upd_gate_len(&s_gate,  &slave, percent_gate_len[1]);
	} 
	else {
		ret = PWM_CV;
	}
	return ret;
}

bool map_uint_to_bool(int mid, unsigned int val){
//	return (val < (mid/2) ? 0 : 1);
	return ((unsigned int) (val / (mid/2)));
}

int bank_trig_level(int sw_state){
	int p1 = get_pot1();
	int p2 = get_pot2();
	int ret = 0;

	if(sw_state == SW_UP){
		m_gate.set_gate_trig_lvl(map_uint_to_bool(MAX_ANALOG_IN, p1));
		s_gate.set_gate_trig_lvl(map_uint_to_bool(MAX_ANALOG_IN, p2));
	}
	else if(sw_state == SW_DOWN){
		if(map_uint_to_bool(MAX_ANALOG_IN, p1))
			ext_trig_level = HIGH;
		else 
			ext_trig_level = LOW;

		if(map_uint_to_bool(MAX_ANALOG_IN, p2))
			slv_trig_level = HIGH;
		else 
			slv_trig_level = LOW;
	}
   	else {
		ret = SLV_CV;
	}
	return ret;
}

int bank_random(int sw_state){

	int pot1 = get_pot1();
	int pot2 = get_pot2();
	int p1 = get_clk_rate(pot1);
	int p2 = rnd_get_max_val(pot2);
	int ret = 0;

	if(sw_state == SW_UP){
		rnds[0].rnd_clk.clk_set_operation(p1, master.clk_get_ms());
		rnds[0].rnd_max = p2;
		rnds[0].rnd_pot = pot2;
	}
	else if(sw_state == SW_DOWN){
		rnds[1].rnd_clk.clk_set_operation(p1, master.clk_get_ms());
		rnds[1].rnd_max = p2;
		rnds[1].rnd_pot = pot2;
	}
	else {
		ret = RANDOM_CV;
	}
	return ret;
}

int bank_all(unsigned int ms){
	int ret = 0;
	/* Save the pot values if not in lock position */
	switch (sw1_state) {
		case SW_UP:
			/* Time */
			ret = bank_time(sw2_state,ms);
			break;
			
		case SW_MID:
			/* burst */
			ret = bank_trig_level(sw2_state);
			break;
	
		case SW_DOWN:
			/* random */
			ret = bank_random(sw2_state);
			break;
	}
	return ret;
}

void upd_output(unsigned int master_ms, unsigned int slave_ms){
	if(master_ms > 0){
		m_gate.rst_gate();
	}
	else {
		m_gate.upd_gate();
	}

	if(slave_ms > 0){
		s_gate.rst_gate();
	} 
	else {
		s_gate.upd_gate();
	}
}



/********************** RANDOM FUNCTIONS *********************************/

static void rnd_init(){
	randomSeed(analogRead(0));	

	rnds[0].rnd_max = MAX_ANALOG_OUT + 1;
	rnds[1].rnd_max = MAX_ANALOG_OUT + 1;

	/* old code start */
//	max_rnd[0] = MAX_ANALOG_OUT + 1;
//	max_rnd[1] = MAX_ANALOG_OUT + 1;
	/* old code end */
}

static void rnd_upd_output(struct rnd_t *r, int pin){
	byte rnd = random(r->rnd_max);
	analogWrite(pin, (255-rnd));
}

static void rnd_handle_cv(struct rnd_t *r, int cv_in){
	int tmp = constrain((r->rnd_pot+cv_in), 0, MAX_ANALOG_IN);
	r->rnd_clk.clk_set_operation(get_clk_rate(tmp), master.clk_get_ms());	
}

static unsigned int rnd_get_max_val(unsigned int in){
	unsigned int ret = map(in, 0, MAX_ANALOG_IN, MIN_ANALOG_OUT, MAX_ANALOG_OUT);
	return ret;
}

/*************************************************************************/



/* old code start */
//static void upd_rnd_output1(){
//	byte rnd = random(max_rnd[0]);
//	analogWrite(aout1, (255-rnd));
//}
//
//static void upd_rnd_output2(){
//	byte rnd = random(max_rnd[1]);
//	analogWrite(aout2, (255 - rnd));
//}
/* old code end */

static void set_slv_cv_gate_len(){
	int att_in = attenuate_input(get_ain2(), get_pot2());

//	int tmp = map(get_ain2(), 0, 1023, 0, 99);
	int tmp = map(att_in, 0, 1023, 0, 99);
	upd_gate_len(&s_gate, &slave, constrain((percent_gate_len[1]+tmp), 0, 99));
}

static void set_slv_cv_mult(){
	int rate;
	int att_in = attenuate_input(get_ain2(), get_pot2());
//	int mult_idx = map(get_ain2(), 0, MAX_ANALOG_IN, 7, (MAX_CLK_SLAVE_RATE-1));
	int mult_idx = map(att_in, 0, MAX_ANALOG_IN, 7, (MAX_CLK_SLAVE_RATE-1));
	if(mult_idx > 7)
		rate = clk_rate[mult_idx];
	else
		rate = abs(slv_mult);

//	int rate = clk_rate[map(get_ain2(), 0, MAX_ANALOG_IN, 7, (MAX_CLK_SLAVE_RATE-1))];
//	slave.clk_set_operation(abs(slv_mult+rate),master.clk_get_ms());

	slave.clk_set_operation(abs(rate),master.clk_get_ms());
	upd_gate_len(&s_gate, &slave, percent_gate_len[1]);
}

static void set_mst_cv_gate_len(){
	int att_in = attenuate_input(get_ain1(), get_pot1());
	
	int tmp = map(att_in, 0, 1023, 0, 99);
	upd_gate_len(&m_gate, &master, constrain((percent_gate_len[0]+tmp), 0, 99));
}





void setup() {
	Serial.begin(9600);
	Serial.println("Begin");

	init_io();
	init_interrupt();
	init_ext_clk();
	init_var();
	rnd_init();
}

void loop(){
	uint32_t rnd_ms[2];
	uint16_t step = master.clk_get_step_cnt();
	unsigned int ms = ckeck_ext_clk();
	int cv_target = bank_all(ms);
	bool slv_clk_triggered = check_slv_trig(slv_trig_level);
	unsigned int slave_ms;
	unsigned int clv_clk_cnt;

	if(!ext_clk_flag){
		ms = master.clk_elapsed();
	} else {
		if(ms > 0){
			/* ext clk: new code start */
			ms = master.clk_sync(ms, (ext_clk.ext_state - 1));
			/* ext clk: new code end */
			/* ext clk: old code start */
//			ms = master.clk_sync(ms, (ext_clk_state - 1));
			/* ext clk: old code end */
		} 
		else {
			ms = master.clk_elapsed();
		}
	}

	if(ms > 0){
		if(cv_target == RANDOM_CV){
//			rnd_handle_cv(&rnds[0], get_ain1());
//			rnd_handle_cv(&rnds[1], get_ain2());
			rnd_handle_cv(&rnds[0], attenuate_input(get_ain1(), get_pot1()));
			rnd_handle_cv(&rnds[1], attenuate_input(get_ain2(), get_pot2()));

		} 
		else if(cv_target == PWM_CV){
			set_mst_cv_gate_len();
		}
	}
	
	if(slv_clk_triggered){
		if(cv_target == PWM_CV) set_slv_cv_gate_len();		
		else if(cv_target == SLV_CV) set_slv_cv_mult();
		slave_ms = slave.clk_reset();
	} 
	else if(slave.clk_get_step_cnt() < (abs(slave.clk_get_operator()) - 1)){
		slave_ms = slave.clk_elapsed();
		// update gate len on new clock
		if( (cv_target == PWM_CV) && slave_ms ) 
			set_slv_cv_gate_len();
	} 
	else {
		slave_ms = 0;
	}

	// Update output
	upd_output(ms, slave_ms);

	if(rnds[0].rnd_clk.master_sync(ms, step) > 0){
		rnd_upd_output(&rnds[0],aout1);
	}
	if(rnds[1].rnd_clk.master_sync(ms, step) > 0){
		rnd_upd_output(&rnds[1],aout2);	
	};

}
