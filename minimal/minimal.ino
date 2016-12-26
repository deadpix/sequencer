#include <Wire.h>
#include <stdlib.h>
//#include <i2c_t3.h>
//#include <SparkFunSX1509.h>
#include "Bounce_array.h"
#include <Bounce2.h>

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
	Wire.setClock(1000000);
}

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
		if(btn[current].update(j)){
			if(btn[current].read(j) == LOW){
				LED_buffer[current][j] = !LED_buffer[current][j];
			} 
		}
	}
	delay(1);
	
	mcp.digitalWrite(btncolumnpins[current], HIGH);
	mcp.digitalWrite(ledcolumnpins[current], HIGH);
	
	for (i = 0; i < NUM_LED_ROWS; i++){
		mcp.digitalWrite(colorpins[i], LOW);
	}
}

static void test_func(){
}

void setup() 
{
	Serial.begin(115200);  
	
	setup_MCP();
	setup_matrix_pins();
	setup_ArrBounce();
	
	next_advance = millis() + 1000;
	led_index = 0;

}

void loop(){
//	test_func();
	for(int i=0;i<NUM_LED_COLUMNS;i++){
		scan(i);
	}
}