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

#include "configuration.h"
#include "src/types.h"
#include "src/hw.h"
#include "src/led_matrix.h"
#include "Bounce_array.h"


#define BTN_NUM_COL		8
#define BTN_NUM_ROW		8
#define BOUNCE_TIME		5

#define LONG_PRESS_TIME_MS	1000
#define NR_CBCK			3


Adafruit_MCP23017 mcp;

struct _btn_status {
	byte pushed_bmp[BTN_NUM_COL];
	byte long_pushed_bmp[BTN_NUM_COL];
};
static struct _btn_status btn_status;


cbck_fct_rd cbck_fct_rd_arr[NR_CBCK];

int init_rd_cbck(cbck_fct_rd fct_ptr, uint8_t fct_id){
	int res = 1;
	if(fct_id < NR_CBCK){
		res = 0;
		cbck_fct_rd_arr[fct_id] = fct_ptr;
	}
	return res;
}
uint8_t mcp_digitalRead(uint8_t pin, uint8_t fct_id){
	return cbck_fct_rd_arr[fct_id](pin);
}





static elapsedMillis btn_ms_cnt[NR_LEDS];

#if HW_SHIFT_REG == 1

#define latchPin 		22
#define clockPin 		23
#define dataPin  		21

#define GRD_OFFSET		0
#define BLUE_OFFSET		16
#define GREEN_OFFSET		24
#define RED_OFFSET		8


static uint8_t btn_select_pins[BTN_NUM_COL] = {3,2,1,0,12,13,14,15}; // ground switch
static uint8_t btn_read_pins[BTN_NUM_ROW] = {7,8,6,9,5,10,4,11};
//static uint8_t btn_select_pins[BTN_NUM_COL] 	= { 7, 6, 5, 4, 3, 2, 1, 0};
//static uint8_t btn_read_pins[BTN_NUM_ROW] 	= {12, 13, 14, 15, 8, 9, 10, 11 };
static ArrBounce btn_row[BTN_NUM_COL];
static uint8_t grd_cnt;

static uint8_t btn_matrix_digitalRead(uint8_t pin){
	return mcp.digitalRead(pin);
}

static void refresh_matrix(uint16_t led_id, uint8_t color){
	
}

static void setup_matrix(){
	int i;
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);

	write_shift_reg(0x0);
	grd_cnt = 0;

//	led_matrix::set_refresh_clbck(refresh_matrix);

	btn_col_idx = 0;
	flag_btn_active = false;

	init_rd_cbck(&btn_matrix_digitalRead, 0);
	mcp.begin(6);
	Wire.setClock(1000000);

	for(i=0;i<BTN_NUM_COL;i++){
		btn_row[i].init_ArrBounce(btn_read_pins, BOUNCE_TIME, BTN_NUM_ROW, &mcp_digitalRead, 0);
		btn_status.pushed_bmp[i] = 0x0;
		btn_status.long_pushed_bmp[i] = 0x0;
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

static void scan(prog* p){
	uint8_t i;

// 	Select current columns
	mcp.digitalWrite(btn_select_pins[btn_col_idx], LOW);

// 	Read the button inputs
	for (i=0; i<BTN_NUM_ROW; i++){
		if(btn_row[btn_col_idx].update(i)){

			if(btn_row[btn_col_idx].read(i) == HIGH){
//				p->on_release(btn_col_idx*BTN_NUM_COL + i);
				flag_btn_active = false;
				if(btn_status.long_pushed_bmp[btn_col_idx] & (1<<i)){
					// release
					p->on_long_release(btn_col_idx*BTN_NUM_COL + i);
				} 
				else {
					p->on_release(btn_col_idx*BTN_NUM_COL + i);
				}
				btn_status.pushed_bmp[btn_col_idx] &= ~(1<<i);
				btn_status.long_pushed_bmp[btn_col_idx] &= ~(1<<i);
				
			} else {
				p->on_push(btn_col_idx*BTN_NUM_COL + i);
				flag_btn_active = true;
				btn_ms_cnt[btn_col_idx*BTN_NUM_COL + i] = 0;
				btn_status.pushed_bmp[btn_col_idx] |= (1<<i);
			}
		}
		if( (btn_status.pushed_bmp[btn_col_idx] & (1<<i)) 
		&& !(btn_status.long_pushed_bmp[btn_col_idx] & (1<<i))){
			if(btn_ms_cnt[btn_col_idx*BTN_NUM_COL + i] > LONG_PRESS_TIME_MS){
				btn_status.long_pushed_bmp[btn_col_idx] |= (1<<i);
				p->on_long_push(btn_col_idx*BTN_NUM_COL + i);
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
	uint32_t tmp = ((1<<(grd_cnt/*+4*/))) << GRD_OFFSET;
	led_t l = lm->get_led(grd_cnt);	
	tmp |= (l.bitmap[LED_COLOR_RED_INDEX] << RED_OFFSET)
		 | (l.bitmap[LED_COLOR_GREEN_INDEX] << GREEN_OFFSET)
		 | (l.bitmap[LED_COLOR_BLUE_INDEX] << BLUE_OFFSET);
	
	write_shift_reg(tmp);
	grd_cnt = (grd_cnt + 1) % LED_MATRIX_NR_GROUND;
}
static void switch_matrix_ui(led_matrix* lm){
}

#elif HW_ADAFRUIT_NEOTRELLIS == 1

#include <Adafruit_NeoTrellis.h>
#define NT_INT_PIN	17

//create a matrix of trellis panels
Adafruit_NeoTrellis t_array[BTN_NUM_ROW/4][BTN_NUM_COL/4] = {
  
  { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F) },
  { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F) }
  
};

Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, BTN_NUM_ROW/4, BTN_NUM_COL/4);

TrellisCallback trellis_btn_clbck(keyEvent evt){
	Serial.print("Triggered event ");
	Serial.println(evt.bit.NUM);
	prog* p = sequenception.current_prog;
	uint8_t row = evt.bit.NUM / BTN_NUM_ROW;	
	uint8_t offset = evt.bit.NUM % BTN_NUM_ROW;

	if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
		flag_btn_active = true;
		p->on_push(evt.bit.NUM);
		btn_ms_cnt[evt.bit.NUM] = 0;
		btn_status.pushed_bmp[row] |= (1<<offset);	
	}
	else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
		flag_btn_active = false;
		if(btn_status.long_pushed_bmp[row] & (1<<offset)){
			// release
			p->on_long_release(evt.bit.NUM);
		} 
		else {
			p->on_release(evt.bit.NUM);
		}
		btn_status.pushed_bmp[row] &= ~(1<<offset);
		btn_status.long_pushed_bmp[row] &= ~(1<<offset);

	}
	if( (btn_status.pushed_bmp[row] & (1<<offset)) 
	&& !(btn_status.long_pushed_bmp[row] & (1<<offset))){
		if(btn_ms_cnt[evt.bit.NUM] > LONG_PRESS_TIME_MS){
			btn_status.long_pushed_bmp[row] |= (1<<offset);
			p->on_long_push(evt.bit.NUM);
		}
	}

	return 0;
}


static void refresh_matrix(uint16_t led_id, uint8_t color){
	
}

static void setup_matrix(){
	if(!trellis.begin()){
		Serial.println("failed to begin trellis");
		while(1);
	}

	flag_btn_active = false;
	pinMode(NT_INT_PIN, INPUT);

  	for(int y=0; y<BTN_NUM_ROW; y++){
		for(int x=0; x<BTN_NUM_COL; x++){
			// activate rising and falling edges on all keys
			trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
			trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
			trellis.registerCallback(x, y, trellis_btn_clbck);
			trellis.setPixelColor(x, y, 0x000000); //addressed with x,y
			trellis.show(); //show all LEDs
    		}
		btn_status.pushed_bmp[y] = 0x0;
		btn_status.long_pushed_bmp[y] = 0x0;
		delay(50);
	}
}
static void scan(prog* p){
	if(!digitalRead(NT_INT_PIN)){
		trellis.read(false);
	}
}
static void upd_shift_reg(led_matrix* lm){
}
static void switch_matrix_ui(led_matrix* lm){
}

#endif
