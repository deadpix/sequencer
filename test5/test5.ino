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

#include "types.h"

#define latchPin 22
#define clockPin 23
#define dataPin  21

#define GRD_OFFSET		24
#define RED_OFFSET		16
#define BLUE_OFFSET		 8
#define GREEN_OFFSET	 0

led_matrix 	current_lm;
uint8_t		grd_cnt;

IntervalTimer ui_timer;

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
	tmp |= (l[LED_COLOR_RED_INDEX] << RED_OFFSET)
		 | (l[LED_COLOR_GREEN_INDEX] << GREEN_OFFSET)
		 | (l[LED_COLOR_BLUE_INDEX] << BLUE_OFFSET);

	Serial.println(tmp);
	write_shift_reg(tmp);

	grd_cnt = (grd_cnt + 1) % LED_MATRIX_NR_GROUND;
}

static void upd_gui(){
	upd_shift_reg(&current_lm);
}

static void test_setup(){
	current_lm.set_led_x(2);
	current_lm.set_led_x(5);
	current_lm.set_led_x(6);
	current_lm.set_led_x(2);
	current_lm.set_led_x(8);
	current_lm.set_led_x(14);
	current_lm.set_led_x(17);
	current_lm.set_led_x(23);
	current_lm.set_led_x(31);
	current_lm.set_led_x(56);
}

void setup_gui(){
	grd_cnt = 0;
}

void setup(){
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	setup_gui();
	test_setup();
	ui_timer.begin(upd_gui, 1000);
}

void loop(){
	delay(5);
}
