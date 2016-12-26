#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>


//#include <Wire.h>
#include <stdlib.h>
#include <i2c_t3.h>
//#include <SparkFunSX1509.h>
#include "Bounce_array.h"
#include <Bounce2.h>

#include <SoftwareSerial.h>
#include <MsTimer2.h>





#define PIN_LED_PLAYING 6
#define PIN_LED_TEMPO 5

//#define PIN_PLAY_INPUT 2
//#define PIN_CONTINUE_INPUT 3

//#define PIN_TEMPO_POT 1

static const uint16_t DEBOUNCE_COUNT = 50;

SoftwareSerial SoftSerial(9, 10);
MIDI_CREATE_INSTANCE(SoftwareSerial, SoftSerial, MIDI);

bool running;
bool send_start;
bool send_stop;
bool send_continue;
bool send_tick;
uint32_t tempo_delay;








#define  MENU_BTN_PIN	10

#define MCP23017	1
#if MCP23017
#include <Adafruit_MCP23017.h>
#endif
#include <elapsedMillis.h>

//config variables
#define NUM_LED_COLUMNS 	(4)
#define NUM_LED_ROWS 		(4)
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

static bool LED_buffer[NUM_LED_COLUMNS][NUM_LED_ROWS];

static int32_t next_advance;
static uint8_t led_index;


static const uint8_t TEENSY_LED_PIN = 13;
static uint8_t btnrowpins[NUM_BTN_ROWS] 				= {3, 2, 1, 0};
//#if MCP23017
static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] 	= {12,13,14,15};
//#else
//static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] 	= {14,15,16,17};	
//#endif
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] 	= {7, 6, 5, 4};
static const uint8_t colorpins[NUM_LED_ROWS]        	= {8, 9, 10, 11};


static ArrBounce btn[NUM_BTN_COLUMNS];

static void setup_MCP(){
	pinMode(TEENSY_LED_PIN, OUTPUT);
	mcp.begin(0);
	Wire.setClock(1200000);
}

static const uint8_t MIDI_DRUM_GM[NUM_BTN_ROWS][NUM_BTN_COLUMNS] = {
	 {37, 36, 42, 82}
	,{40, 38, 46, 44}
	,{48, 47, 45, 43}
	,{49, 55, 51, 53}
};


/* 
 *	WARNING: with MCP23017, io_digitalRead can be called AFTER
 * 	Adafruit_MCP23017 object has been initialized 
 */
static uint8_t io_digitalRead(uint8_t pin){
#if MCP23017
	return mcp.digitalRead(pin);
#else 
	return digitalRead(pin);
#endif
}

static void setup_ArrBounce(){
	for(int i=0;i<NUM_BTN_COLUMNS;i++){
		btn[i].init_ArrBounce(btnrowpins, BOUNCE_TIME, NUM_BTN_ROWS, &io_digitalRead);
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
	}

	// button row input lines
	for (i = 0; i < NUM_BTN_ROWS; i++){
		mcp.pinMode(btnrowpins[i], INPUT);
		mcp.pullUp(btnrowpins[i], HIGH);
	}

	// LED drive lines
	for (i = 0; i < NUM_LED_ROWS; i++){
		mcp.pinMode(colorpins[i], OUTPUT);
		mcp.digitalWrite(colorpins[i], LOW);
	}	
}


struct note_state {
	uint8_t trig;
	uint8_t state;
};

struct note_state pad_state[NUM_BTN_ROWS];
uint16_t dim_led_cnt[NUM_BTN_ROWS][NUM_BTN_COLUMNS];
#define DIM_LED_LIMIT	10
#define NOTE_ON		1
#define NOTE_OFF	0

static boolean pad_event;

static void scan(uint8_t current){
	uint8_t val;
	uint8_t i, j;

// 	Select current columns
	mcp.digitalWrite(btncolumnpins[current], LOW);
	mcp.digitalWrite(ledcolumnpins[current], LOW);

//	output LED row values
	for (i = 0; i < NUM_LED_ROWS; i++){
		if (LED_buffer[current][i]){
			mcp.digitalWrite(colorpins[i], HIGH);
		}
	}

// 	pause a moment
	delay(1);

// 	Read the button inputs
	for ( j = 0; j < NUM_BTN_ROWS; j++){
//	LED_buffer[current][j] = true;
		if(btn[current].update(j)){
			
			pad_event = true;
		

			if(btn[current].read(j) == LOW){
//				LED_buffer[current][j] = !LED_buffer[current][j];

				dim_led_cnt[current][j] = 0;
				LED_buffer[current][j] = true;
				pad_state[current].trig |= (1 << j);
				pad_state[current].state |= (NOTE_ON << j);
			
			} else {
				pad_state[current].state &= ~(NOTE_ON << j);
			}			
			
		}
		if(dim_led_cnt[current][j] > DIM_LED_LIMIT){
			LED_buffer[current][j] = false;
		} 
		else {
			dim_led_cnt[current][j]++;
		}

	}
	delay(1);
	
	mcp.digitalWrite(btncolumnpins[current], HIGH);
	mcp.digitalWrite(ledcolumnpins[current], HIGH);
	
	for (i = 0; i < NUM_LED_ROWS; i++){
		mcp.digitalWrite(colorpins[i], LOW);
	}
}



#define CHANNEL 	1
static void play_triggered_pad(uint8_t row, struct note_state* row_state){
	
	for(int i=0;i<NUM_BTN_COLUMNS;i++){
		if( (row_state->trig >> i) & 0x1 ){
			if((row_state->state >> i) & 0x1){
				// send midi on 
				MIDI.sendNoteOn(MIDI_DRUM_GM[row][i], 127, CHANNEL);
			} 
			else {
				// send midi off
				MIDI.sendNoteOff(MIDI_DRUM_GM[row][i], 0, CHANNEL);
				// clear trig bitmap
				row_state->trig &= ~(1 << i);
			}
		}
	}
}




static void play_button_event(){
  // toggle running state, 
  // send corresponding responses
	
  running = !running;
    
    if(running)
    {
      send_start = true;
      digitalWrite(PIN_LED_PLAYING, LOW);
    }
    else
    {
      send_stop = true;
      digitalWrite(PIN_LED_PLAYING, HIGH);
    }
}

void cont_button_event()
{
  // ignore continue if running
  if(!running)
  {
    send_continue = true;
    running = true;
    digitalWrite(PIN_LED_PLAYING, LOW);
  }
}

void timer_callback()
{
  send_tick = true;
}
/*
void check_pots()
{
  uint32_t pot_val;
  uint32_t calc;
  
  pot_val = analogRead(PIN_TEMPO_POT);
  
  // Result is 10 bits
  calc = (((0x3ff - pot_val) * 75)/1023) + 8;
  
  tempo_delay = calc  ;//* 5;
}
*/
/*
void check_buttons()
{
  uint8_t val;
  static uint16_t play_debounce = 0;
  static uint16_t cont_debounce = 0;

  // First the PLAY/STOP button  
  val = digitalRead(PIN_PLAY_INPUT);
  
  if(val == LOW)
  {
    play_debounce++;
    
    if(play_debounce == DEBOUNCE_COUNT)
    {
      play_button_event();
    }
  }
  else
  {
    play_debounce = 0;
  }

  // Then the continue button
  val = digitalRead(PIN_CONTINUE_INPUT);
  
  if(val == LOW)
  {
    cont_debounce++;
    
    if(cont_debounce == DEBOUNCE_COUNT)
    {
      cont_button_event();
    }
  }
  else
  {
    cont_debounce = 0;
  }
}
*/

void setup_midi(){
	pinMode(PIN_LED_PLAYING, OUTPUT);
	pinMode(PIN_LED_TEMPO, OUTPUT);
	digitalWrite(PIN_LED_PLAYING, HIGH);
	digitalWrite(PIN_LED_TEMPO, HIGH);
	
	SoftSerial.begin(31250);
	  // do I need to init the soft serial port?
  
#if 1
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
#endif  

  running = false;
  send_start = false;
  send_stop = false;
  send_tick = false;
	pad_event = false;

	tempo_delay = 10;
  MsTimer2::set(tempo_delay, timer_callback);
  MsTimer2::start();

}











void setup() 
{
	Serial.begin(115200);  
	
	setup_MCP();
	setup_matrix_pins();
	setup_ArrBounce();
	setup_midi();

	send_start = true;
	
	next_advance = millis() + 1000;
	led_index = 0;

}

static uint32_t loops = 0;  
static uint8_t  ticks = 0;
static uint8_t  prev_ticks = 0;

static uint8_t line;
void loop(){	
	Serial.println(ms);
	ms = 0;
	MIDI.read();
	elapsedMillis ms_cnt=0;

//	Check buttons
//	for(int i=0;i<4;i++){
		scan(line);
//	}
		
	if(pad_event){
		play_triggered_pad(line, &pad_state[line]);
		pad_event = false;
	}
	bool reset_timer = false;

	// turn the crank...

  // Check buttons
//  check_buttons();

/*
  // process inputs
	if(send_start){
		MIDI.sendRealTime(MIDI_NAMESPACE::Start);
		send_start = false;
		ticks = 0;
       
    // Next tick comes immediately...
    // it also resets the timer
		send_tick = true;
    
  }
*/
/*  
  if(send_continue)
  {
    MIDI.sendRealTime(MIDI_NAMESPACE::Continue);
    send_continue = false;
    Serial.println("continuing");

    // Restore the LED blink counter
    ticks = prev_ticks;
    
    // Next tick comes immediately...
    // it also resets the timer
    send_tick = true;
  }
*/ 
/*
  if(send_stop)
  {
    MIDI.sendRealTime(MIDI_NAMESPACE::Stop);
    send_stop = false;
    prev_ticks = ticks ;
    Serial.println("Stopping");
  }
*/
	if(send_tick){
		MIDI.sendRealTime(MIDI_NAMESPACE::Clock);
		send_tick = false;
    
		ticks++;
    
		if(ticks < 6){
			digitalWrite(PIN_LED_TEMPO, LOW);
		}
		else if(ticks == 6){
			digitalWrite(PIN_LED_TEMPO, HIGH);
		}
		else if(ticks >= 24){
			ticks = 0;
		}
   		reset_timer = true;
	}

	if(reset_timer){
		MsTimer2::stop();
		MsTimer2::set(tempo_delay, timer_callback);
		MsTimer2::start();
		
		reset_timer = false;
	}
	loops++;
	line = (line + 1) % NUM_BTN_ROWS;
}
