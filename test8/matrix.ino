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
#include <i2c_t3.h>
#include <Adafruit_MCP23017.h>

#include "types.h"
#include "led_matrix.h"
#include "Bounce_array.h"
#include "matrixUI.h"

#define latchPin 		22
#define clockPin 		23
#define dataPin  		21

#define GRD_OFFSET		0
#define BLUE_OFFSET		16
#define GREEN_OFFSET	24
#define RED_OFFSET		8


//#define GRD_OFFSET		24
//#define BLUE_OFFSET		16
//#define GREEN_OFFSET	 8
//#define RED_OFFSET		 0

#define BTN_NUM_COL		8
#define BTN_NUM_ROW		8
#define BOUNCE_TIME		5

#define DEBUG 1

Adafruit_MCP23017 mcp;
uint8_t		grd_cnt;


static uint8_t btn_select_pins[BTN_NUM_COL] = {3,2,1,0,12,13,14,15}; // ground switch
static uint8_t btn_read_pins[BTN_NUM_ROW] = {7,8,6,9,5,10,4,11};


//static uint8_t btn_select_pins[BTN_NUM_COL] 	= { 7, 6, 5, 4, 3, 2, 1, 0};
//static uint8_t btn_read_pins[BTN_NUM_ROW] 	= {12, 13, 14, 15, 8, 9, 10, 11 };

static ArrBounce btn_row[BTN_NUM_COL];

static uint8_t btn_matrix_digitalRead(uint8_t pin){
	return mcp.digitalRead(pin);
}
static void init_matrix_btn(){
	int i;
	btn_col_idx = 0;

	mcp.begin(6);
	Wire.setClock(1000000);

	for(i=0;i<BTN_NUM_COL;i++){
		btn_row[i].init_ArrBounce(btn_read_pins, BOUNCE_TIME, BTN_NUM_ROW, &btn_matrix_digitalRead);
	}

	for (i=0;i<BTN_NUM_COL;i++){
		mcp.pinMode(btn_select_pins[i], OUTPUT);
		mcp.digitalWrite(btn_select_pins[i], HIGH);
	}

	// button row input lines
	for (i=0;i<BTN_NUM_ROW;i++){
		mcp.pinMode(btn_read_pins[i], INPUT);
		mcp.pullUp(btn_read_pins[i], HIGH);
	}
}

static void scan(const matrixUI* const mui){
	uint8_t i;

// 	Select current columns
	mcp.digitalWrite(btn_select_pins[btn_col_idx], LOW);

// 	Read the button inputs
	for (i=0; i<BTN_NUM_ROW; i++){
		if(btn_row[btn_col_idx].update(i)){

			if(btn_row[btn_col_idx].read(i) == HIGH){
				mui->on_release(btn_col_idx*BTN_NUM_COL + i);
			} else {
				mui->on_push(btn_col_idx*BTN_NUM_COL + i);
#if DEBUG
				Serial.print("push btn ");
				Serial.println(btn_col_idx*BTN_NUM_COL + i);
#endif
			}
		}
	}
	mcp.digitalWrite(btn_select_pins[btn_col_idx], HIGH);	
	btn_col_idx = (btn_col_idx+1)%BTN_NUM_COL;
}
static void write_shift_reg(uint32_t val){
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, val);
	shiftOut(dataPin, clockPin, LSBFIRST, val >> 8);
	shiftOut(dataPin, clockPin, LSBFIRST, val >> 16);
	shiftOut(dataPin, clockPin, LSBFIRST, val >> 24);
	digitalWrite(latchPin, HIGH);
}

static void upd_shift_reg(const led_matrix* const lm){
	uint32_t tmp = ((1<<(grd_cnt/*+4*/))) << GRD_OFFSET;
	led_t l = lm->get_led(grd_cnt);	
	tmp |= (l.bitmap[LED_COLOR_RED_INDEX] << RED_OFFSET)
		 | (l.bitmap[LED_COLOR_GREEN_INDEX] << GREEN_OFFSET)
		 | (l.bitmap[LED_COLOR_BLUE_INDEX] << BLUE_OFFSET);
	
	write_shift_reg(tmp);
	grd_cnt = (grd_cnt + 1) % LED_MATRIX_NR_GROUND;
}
static void setup_gui(){
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);

	write_shift_reg(0x0);
	grd_cnt = 0;
}
