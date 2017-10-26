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

//#include <Wire.h>
#include <i2c_t3.h>
#include <Adafruit_MCP23017.h>

#include "Bounce_array.h"
#include "types.h"
#include "led_matrix.h"

#define latchPin 		22
#define clockPin 		23
#define dataPin  		21

//#define GRD_OFFSET		24
//#define BLUE_OFFSET		16
//#define GREEN_OFFSET	 8
//#define RED_OFFSET		 0

#define GRD_OFFSET		0
#define BLUE_OFFSET		16
#define GREEN_OFFSET	24
#define RED_OFFSET		8


led_matrix 	current_lm;
uint8_t		grd_cnt;

IntervalTimer ui_timer;

Adafruit_MCP23017 mcp;
IntervalTimer btn_timer;

uint8_t	drive_cnt;
//static uint8_t btn_drive_pins[BTN_MATRIX_NR_COL_GND] = {15, 14, 13, 12, 0, 1, 2, 3}; // ground switch
//static uint8_t btn_read_pins[BTN_MATRIX_NR_ROW_SW] = {11, 10, 9, 8, 4, 5, 6, 7};
//static uint8_t btn_drive_pins[BTN_MATRIX_NR_COL_GND] = {0,1,2,3,15,14,13,12}; // ground switch
static uint8_t btn_drive_pins[BTN_MATRIX_NR_COL_GND] = {3,2,1,0,12,13,14,15}; // ground switch
//static uint8_t btn_read_pins[BTN_MATRIX_NR_ROW_SW] = {9,6,8,7,11,4,10,5};
static uint8_t btn_read_pins[BTN_MATRIX_NR_ROW_SW] = {7,8,6,9,5,10,4,11};
//static uint8_t btn_read_pins[BTN_MATRIX_NR_ROW_SW] = {8,7,9,6,10,5,11,4};



static ArrBounce btn[BTN_MATRIX_NR_COL_GND];
volatile bool flg_btn_matrix;


/* LEDS MATRIX */

static void write_shift_reg(uint32_t val){
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, val);
	shiftOut(dataPin, clockPin, LSBFIRST, val >> 8);
	shiftOut(dataPin, clockPin, LSBFIRST, val >> 16);
	shiftOut(dataPin, clockPin, LSBFIRST, val >> 24);
	digitalWrite(latchPin, HIGH);
}

static void upd_shift_reg(led_matrix* lm){
	uint32_t tmp = (1<<grd_cnt) << GRD_OFFSET;
	led_t l = lm->get_led(grd_cnt);	
	tmp |= (l.bitmap[LED_COLOR_RED_INDEX] << RED_OFFSET)
		 | (l.bitmap[LED_COLOR_GREEN_INDEX] << GREEN_OFFSET)
		 | (l.bitmap[LED_COLOR_BLUE_INDEX] << BLUE_OFFSET);

	write_shift_reg(tmp);
	grd_cnt = (grd_cnt + 1) % LED_MATRIX_NR_GROUND;
}


/* BUTTONS MATRIX */
static uint8_t btn_digitalRead(uint8_t pin){
	return mcp.digitalRead(pin);
}
static void scan_btn_matrix(){
	if(flg_btn_matrix){
	//	Serial.println("scan");
		mcp.digitalWrite(btn_drive_pins[drive_cnt], LOW);
		for(int i=0;i<BTN_MATRIX_NR_ROW_SW;i++){
			if(btn[drive_cnt].update(i)){
				if(btn[drive_cnt].read(i) == LOW){
//					Serial.print("btn ");
					Serial.println((drive_cnt*8+i));
//					Serial.println(" released");
					current_lm.toogle_led_x(LED_COLOR_RED_INDEX,((drive_cnt*8+i)));
				}
				else {
//					Serial.print("btn ");
//					Serial.print((drive_cnt*i));
//					Serial.println(" pushed");	
				}
			}
		}
		mcp.digitalWrite(btn_drive_pins[drive_cnt], HIGH);

		drive_cnt = (drive_cnt + 1) % BTN_MATRIX_NR_COL_GND;
		flg_btn_matrix = false;
	}
}
static void init_btn_matrix(){
	int i;

	/* init IO expander */
	mcp.begin(6);
	Wire.setClock(1000000);

	for (i = 0; i < BTN_MATRIX_NR_COL_GND; i++){ 
		mcp.pinMode(btn_drive_pins[i], OUTPUT);
		mcp.digitalWrite(btn_drive_pins[i], HIGH);
	}
	for (i = 0; i < BTN_MATRIX_NR_ROW_SW; i++){ 
		mcp.pinMode(btn_read_pins[i], INPUT);
		mcp.pullUp(btn_read_pins[i], HIGH);
	}
	for(i=0; i<BTN_MATRIX_NR_COL_GND; i++){
		btn[i].init_ArrBounce(btn_read_pins, BOUNCE_TIME, BTN_MATRIX_NR_ROW_SW, &btn_digitalRead);
	}
	flg_btn_matrix = false;
}


/* IRQ callback functions */
static void upd_gui(){
	upd_shift_reg(&current_lm);
}
static void scan_btns(){
	if(!flg_btn_matrix)
		flg_btn_matrix = true;
}
static void init_interrupt(){
	ui_timer.priority(128);
	btn_timer.priority(255);	
	ui_timer.begin(upd_gui, 1000);	
	btn_timer.begin(scan_btns, 10000);
}












static void all_leds(uint8_t color){
	for(int i=0;i<NR_LEDS;i++){
		current_lm.set_led_x(color, i);
	}
}

static void test_setup(){
	current_lm.set_led_x(LED_COLOR_RED_INDEX,2);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,5);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,6);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,8);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,14);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,17);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,22);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,23);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,31);
	current_lm.set_led_x(LED_COLOR_RED_INDEX,56);
}

static void setup_gui(){
	grd_cnt = 0;
}

static void test_toogle_led(int color, int led_idx){
	current_lm.toogle_led_x(color,led_idx);
}





int cnt;
int color_idx;
int toogle_idx;
int color_array[LED_MATRIX_NR_COLORS] = {LED_COLOR_BLUE_INDEX,LED_COLOR_GREEN_INDEX,LED_COLOR_RED_INDEX};

void setup(){
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	setup_gui();
//	test_setup();

	init_btn_matrix();
	init_interrupt();
	
	cnt = 0;
	color_idx = 0;
	toogle_idx = 0;
}

static void test_led_matrix(){
	while(color_idx < LED_MATRIX_NR_COLORS){
		Serial.print("color ");
		Serial.println(color_idx);
		while(toogle_idx < 2){
			Serial.print("toogle ");
			Serial.println(toogle_idx);
			while(cnt < NR_LEDS){
				test_toogle_led(color_array[color_idx],cnt);
				delay(50);
				cnt++;
			}
			cnt = 0;
			toogle_idx++;
		}
		toogle_idx = 0;
		color_idx++;
	}
	color_idx = 0;
	delay(1000);
}

void loop(){
//	test_setup();	
//	Serial.println("START");
//	current_lm.dump_led_matrix();
	scan_btn_matrix();
//	uint32_t val = 0xff0000ff;
	
//	write_shift_reg(val);
//	test_led_matrix();
}
