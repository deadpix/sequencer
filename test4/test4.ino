#include <Wire.h>
#include <stdlib.h>
//#include <i2c_t3.h>
//#include <SparkFunSX1509.h>
#include "Bounce_array.h"
#include <Bounce2.h>

#define  MENU_BTN_PIN	10

#define MCP23017	1
//#if MCP23017
#include <Adafruit_MCP23017.h>
//#endif

#include <elapsedMillis.h>

#include "clock.h"
#include "step.h"
#include "seq.h"
#include "led_matrix.h"
#include "btn_action.h"
#include "menu.h"
#include "sequencer.h"
#include "global.h"

//#include "led.h"

//config variables
//#define NUM_LED_COLUMNS 	(4)
//#define NUM_LED_ROWS 		(4)
#define NUM_BTN_COLUMNS 	(4)
#define NUM_BTN_ROWS 		(4)
#define NUM_COLORS 			(1)
#define MAX_DEBOUNCE 		(3)
#define BOUNCE_TIME			(5)

#define EXTERNAL_CLOCK_PIN	(20)
#define CLOCK_OUT_PIN		(21)

// debug
#define GET_LATENCY			(0)



//SX1509 io;
Adafruit_MCP23017 mcp;
elapsedMillis ms;
global global_setting;


//static bool LED_buffer[NUM_LED_COLUMNS][NUM_LED_ROWS];
//static led_matrix LED_buffer;
led_matrix* led_buf;

static int32_t next_advance;
static uint8_t led_index;


static const uint8_t TEENSY_LED_PIN = 13;
static uint8_t btnrowpins[NUM_BTN_ROWS] 				= {3, 2, 1, 0};
#if MCP23017
static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] 	= {12,13,14,15};
#else
static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] 	= {14,15,16,17};	
#endif
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] 	= {7, 6, 5, 4};
static const uint8_t colorpins[NUM_LED_ROWS]        	= {8, 9, 10, 11};

static const uint8_t seq_pinout[NUM_LED_COLUMNS] = { 21, 22, 23, 24};

volatile boolean external_clock;
volatile boolean internal_clock = true;

clock master;
clock slave_div;
clock slave_mul;

btn_action matrix_btn;

prog *prog_arr[NUM_LED_COLUMNS];

void external_clock_handler(){
	if(!internal_clock){
		master.sync_extern();
		external_clock = true;
	}
}

static ArrBounce btn[NUM_BTN_COLUMNS];
static Bounce menu_btn;
static menu   menu_manager;
static sequencer drum_seq;
static seq s;
//led_matrix menu_matrix;

static void setup_MCP(){
	pinMode(TEENSY_LED_PIN, OUTPUT);
	mcp.begin(0);
	Wire.setClock(1000000);
}

/* 
 *	WARNING: with MCP23017, io_digitalRead can be called AFTER
 * 	Adafruit_MCP23017 object has been initialized 
 */
static uint8_t io_digitalRead(uint8_t pin){
//	Serial.println(pin);
#if MCP23017
	return mcp.digitalRead(pin);
#else 
	return digitalRead(pin);
#endif
}

static void seq_digitalWrite(uint8_t seq_id, boolean val){
	digitalWrite(seq_pinout[seq_id], val);
}

static void setup_menu(){
	pinMode(MENU_BTN_PIN, INPUT_PULLUP);
	menu_btn.attach(MENU_BTN_PIN);
	menu_btn.interval(BOUNCE_TIME);
	
	led_matrix* menu_matrix = menu_manager.get_menu_led_matrix();
		
	// test
	global_setting.set_prog_id(0);
	menu_manager.set_menu_clbk(0,global_setting.menu_clbk,&global_setting);
	menu_matrix->set_pixel_status(0 * NUM_BTN_ROWS, global_setting.play);
	
	drum_seq.set_prog_id(1);
	menu_manager.set_menu_clbk(1,drum_seq.menu_clbk,&drum_seq);
	menu_matrix->set_pixel(1 * NUM_BTN_ROWS + 0);
	
	prog_arr[0] = &global_setting;	
	prog_arr[1] = &drum_seq;
	prog_arr[2] = &menu_manager;

	
}

static void setup_global_setting(){
	global_setting.play = true;
}

static void setup_ArrBounce(){
	for(int i=0;i<NUM_BTN_COLUMNS;i++){
		btn[i].init_ArrBounce(btnrowpins, BOUNCE_TIME, NUM_BTN_ROWS, &io_digitalRead);
//		btn[i].init_ArrBounce(btnrowpins, BOUNCE_TIME, NUM_BTN_ROWS, std::function(&Adafruit_MCP23017::digitalRead, this));
	}
}

static void setup_clock(){
//	master = new clock();
//	slave_div.setFactor(-2);
//	slave_mul.setFactor(2);
	pinMode(EXTERNAL_CLOCK_PIN, INPUT);
	pinMode(CLOCK_OUT_PIN, INPUT);
	attachInterrupt(EXTERNAL_CLOCK_PIN, external_clock_handler, RISING);
}

static void setup_seq(btn_action* btn){
	seq *s;
	prog* tmp = prog_arr[1];
	matrix_btn.set_current_prog(tmp);
	drum_seq.set_current_seq(0);
	
	for(int i=0;i<drum_seq.nr_seq;i++){
		s = (drum_seq.get_seq(i));
		s->init_hw_clbk(seq_digitalWrite, i);

		if(i == 0)
			s->unmute();
		else 
			s->mute();
//		btn->setup_btn_action( s->callback_on_push, s->callback_on_release, s );
//		btn->setup_btn_action_test(tmp->test_on_push, tmp->test_on_release, prog_arr[0]);
		
	}
	
}

static void setup_matrix_pins()
{
	uint8_t i;
	
	for (i = 0; i < NUM_LED_COLUMNS; i++){
		mcp.pinMode(ledcolumnpins[i], OUTPUT);
		mcp.digitalWrite(ledcolumnpins[i], HIGH);
	}

	// button columns
	for (i = 0; i < NUM_BTN_COLUMNS; i++){
		mcp.pinMode(btncolumnpins[i], OUTPUT);
		mcp.digitalWrite(btncolumnpins[i], HIGH);
//		pinMode(btncolumnpins[i], OUTPUT);
//		digitalWrite(btncolumnpins[i], HIGH);
	}

	// button row input lines
	for (i = 0; i < NUM_BTN_ROWS; i++){
		mcp.pinMode(btnrowpins[i], INPUT);
		mcp.pullUp(btnrowpins[i], HIGH);
//		mcp.pinMode(btnrowpins[i], INPUT_PULLUP);
//		pinMode(btnrowpins[i], INPUT_PULLUP);
	}

	// LED drive lines
	for (i = 0; i < NUM_LED_ROWS; i++){
		mcp.pinMode(colorpins[i], OUTPUT);
		mcp.digitalWrite(colorpins[i], LOW);
	}	
}


static void scan(uint8_t current){
//	static uint8_t current = 0;
	uint8_t val;
	uint8_t i, j;

// 	Select current columns
	mcp.digitalWrite(btncolumnpins[current], LOW);
//	digitalWrite(btncolumnpins[current], LOW);
	mcp.digitalWrite(ledcolumnpins[current], LOW);

//	output LED row values
	for (i = 0; i < NUM_LED_ROWS; i++){
		//if (LED_buffer[current][i]){
		if(led_buf->get_pixel_status(current*NUM_LED_ROWS+i) == LED_ON){
			mcp.digitalWrite(colorpins[i], HIGH);

#if GET_LATENCY
			Serial.print("Latency ");
			Serial.println(ms);
			ms = 0;
#endif
		}
	}

// 	pause a moment
	delay(1);

// 	Read the button inputs
	for ( j = 0; j < NUM_BTN_ROWS; j++){
		if(btn[current].update(j)){
			prog* tmp = matrix_btn.get_current_prog();
			if(btn[current].read(j) == LOW){
//				LED_buffer[current][j] = !LED_buffer[current][j];
//				led_buf->toogle_pixel_status(current,j);
//				s.toogle_step_x(current*NUM_LED_COLUMNS + j);
//				matrix_btn.btn_on_release(matrix_btn.object_ptr, current*NUM_LED_COLUMNS + j);
				tmp->on_release(tmp, current*NUM_LED_COLUMNS + j);
			} else {
//				matrix_btn.btn_on_push(matrix_btn.object_ptr, current*NUM_LED_COLUMNS + j);
				tmp->on_push(tmp, current*NUM_LED_COLUMNS + j);
			}
		}
	}
	delay(1);
	
	mcp.digitalWrite(btncolumnpins[current], HIGH);
//	digitalWrite(btncolumnpins[current], HIGH);
	mcp.digitalWrite(ledcolumnpins[current], HIGH);
	
	for (i = 0; i < NUM_LED_ROWS; i++){
		mcp.digitalWrite(colorpins[i], LOW);
	}
}

elapsedMillis gate_out_ms;
static void upd_out(boolean gate){
	digitalWrite(CLOCK_OUT_PIN, gate);
} 

static void test_seq2(){
	// 
	if(global_setting.play){
		drum_seq.update_out(&master);
		drum_seq.update_gui();
	}
//	boolean clock_flag = master.clock_next_step(&gate_flag);	
//	boolean clock_flag;
//	seq *s = drum_seq.get_seq(0);
//	s->check_event(clock_flag, &master);
//	uint8_t curr_step = s->get_current_step();
//	(s->get_clk())->clock_upd_gui(s->get_current_step(), s->get_led_matrix(), s->is_step_on(curr_step));
}

boolean display;

void setup() 
{
	Serial.begin(115200);  

	setup_global_setting();
	setup_MCP();
	setup_matrix_pins();
	setup_ArrBounce();
	setup_menu();
	setup_seq(&matrix_btn);
	display = true;
	
	led_buf = drum_seq.get_seq(0)->get_led_matrix();
	setup_clock();
	
	next_advance = millis() + 1000;
	led_index = 0;

	menu_manager.set_next_interface(drum_seq.get_seq(0)->get_led_matrix());
}

void loop(){
	
	if(menu_btn.update()){
		if(menu_btn.rose()){
			
			led_matrix* lm = menu_manager.get_next_interface();
			if(lm)
				led_buf = lm;
//			seq *s = drum_seq.get_seq(0);
			matrix_btn.set_current_prog(prog_arr[1]);
		
//			led_buf = s->get_led_matrix();
//			matrix_btn.setup_btn_action( s->callback_on_push, s->callback_on_release, s );

		}
		else {
			led_buf = menu_manager.get_menu_led_matrix();
			matrix_btn.set_current_prog(prog_arr[2]);
//			matrix_btn.setup_btn_action( menu_manager.callback_on_push, menu_manager.callback_on_release, &menu_manager );
//			master.clock_upd_gui(0, menu_manager.get_menu_led_matrix(), false);

		}
	}
	test_seq2();
	for(int i=0;i<NUM_LED_COLUMNS;i++){
		scan(i);
	}
}