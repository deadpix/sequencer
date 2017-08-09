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
#include "clk.h"

#define latchPin 		22
#define clockPin 		23
#define dataPin  		21

#define GRD_OFFSET		24
#define BLUE_OFFSET		16
#define GREEN_OFFSET	 8
#define RED_OFFSET		 0

#define BTN_NUM_COL		8
#define BTN_NUM_ROW		8
#define BOUNCE_TIME		5


led_matrix 	current_lm;
uint8_t		grd_cnt;

clk mst_clk;
volatile bool check_clk;

Adafruit_MCP23017 mcp;

static uint8_t btn_select_pins[BTN_NUM_COL] 	= { 7, 6, 5, 4, 3, 2, 1, 0};
//static uint8_t btn_select_pins[BTN_NUM_COL] 	= { 7,6,5,4};
//static const btn_to_led_id[BTN_NUM_COL*BTN_NUM_ROW] = {3,2,1,0,7,6,5,4,};
//static uint8_t btn_select_pins[BTN_NUM_COL] 	= { 7,6,5,4};
static uint8_t btn_read_pins[BTN_NUM_ROW] 	= {12, 13, 14, 15, 8, 9, 10, 11 };
//static uint8_t btn_read_pins[BTN_NUM_ROW] 	= {8, 9, 10};
//static uint8_t btn_read_pins[BTN_NUM_ROW] 	= {15,14,13,12,11,10,9,8};
static ArrBounce btn_row[BTN_NUM_COL];
uint8_t btn_col_idx;
volatile bool btn_flag;

IntervalTimer ui_timer;
IntervalTimer btn_timer;

static uint8_t btn_matrix_digitalRead(uint8_t pin){
	return mcp.digitalRead(pin);
}


static void check_btn(){
	if(!btn_flag)
		btn_flag = true;
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

static void scan(){
	uint8_t i, j;

// 	Select current columns
	mcp.digitalWrite(btn_select_pins[btn_col_idx], LOW);

// 	Read the button inputs
	for (i=0; i<BTN_NUM_ROW; i++){
		if(btn_row[btn_col_idx].update(i)){

			if(btn_row[btn_col_idx].read(i) == LOW){
				Serial.print("button ");
				Serial.print(btn_col_idx*BTN_NUM_COL+i);
				Serial.println(" pushed");
			} else {
				current_lm.toogle_led_x(LED_COLOR_GREEN_INDEX,btn_col_idx*BTN_NUM_COL+i);
				Serial.print("button ");
				Serial.print(btn_col_idx*BTN_NUM_COL+i);
				Serial.println(" released");
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

static void upd_shift_reg(led_matrix* lm){
	uint32_t tmp = ((1<<(grd_cnt+4))) << GRD_OFFSET;
	led_t l = lm->get_led(grd_cnt);	
	tmp |= (l.bitmap[LED_COLOR_RED_INDEX] << RED_OFFSET)
		 | (l.bitmap[LED_COLOR_GREEN_INDEX] << GREEN_OFFSET)
		 | (l.bitmap[LED_COLOR_BLUE_INDEX] << BLUE_OFFSET);
	
	write_shift_reg(tmp);
	grd_cnt = (grd_cnt + 1) % LED_MATRIX_NR_GROUND;
}

static void upd_gui(){
	upd_shift_reg(&current_lm);
	if(!check_clk)
		check_clk = true;
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
	write_shift_reg(0x0);
	grd_cnt = 0;
}

static void test_toogle_led(int color, int led_idx){
	current_lm.toogle_led_x(color,led_idx);
}





int cnt;
int color_idx;
int toogle_idx;
int color_array[LED_MATRIX_NR_COLORS] = {LED_R_IDX,LED_G_IDX,LED_B_IDX};

static void test_led_matrix(){
	while(color_idx < LED_MATRIX_NR_COLORS){
		while(toogle_idx < 2){
			while(cnt < NR_LEDS){
				test_toogle_led(color_array[color_idx],cnt);
				delay(200);
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

static void next_step(clk* c){
	uint32_t ms = c->clk_elapsed();
	if(ms > 0){
		uint16_t cnt = c->clk_get_step_cnt();
		if(cnt == 0)
			current_lm.clr_led_x(LED_G_IDX,31);
		else 
			current_lm.toogle_led_x(LED_G_IDX,(cnt-1));

		current_lm.toogle_led_x(LED_G_IDX,c->clk_get_step_cnt());
	}
}

void setup(){
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	setup_gui();
	init_matrix_btn();

	ui_timer.begin(upd_gui, 1000);
	btn_timer.begin(check_btn, 1000);
	
	cnt = 0;
	color_idx = 0;
	toogle_idx = 0;

	mst_clk.clk_set_max_step(32);
}


void loop(){
	uint32_t ms;
	if(check_clk){
		next_step(&mst_clk);
		check_clk = false
	}

//	test_led_matrix();
//	if(btn_flag){
////		Serial.println("TEST");
//		scan();
//	}
}
