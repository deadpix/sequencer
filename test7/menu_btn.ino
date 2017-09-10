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
#include "Bounce_array.h"
#include <Bounce2.h>
#include "prog.h"

#define NR_MENU_BTN				4
#define MENU_BTN_BOUNCE_TIME	5
#define PIN_MENU_BTN			17

Bounce menu_btn = Bounce();
//menu menu_ctrl;

static uint8_t btn_menu_pins[NR_MENU_BTN] = {7, 6, 5, 4};
Adafruit_MCP23017 mcp_menu_btn;

static ArrBounce bn_menu_btn;

static uint8_t btn_menu_digitalRead(uint8_t pin){
	return mcp_menu_btn.digitalRead(pin);
}

void set_prog_menu_entry(uint8_t id, prog* prog){
	menu_ctrl.set_menu_prog_entry(id, prog);
}

static void init_menu_btn(prog* p){
	menu_btn;
	pinMode(PIN_MENU_BTN,INPUT);
	menu_btn.attach(PIN_MENU_BTN);
	menu_btn.interval(MENU_BTN_BOUNCE_TIME);
//	menu_ctrl.set_next_interface(p->get_led_matrix());
	menu_ctrl.set_next_prog(p);
//	mcp_menu_btn.begin(0);
//	Wire.setClock(1000000);

//
//	mcp_menu_btn.pinMode(7, INPUT);
//	mcp_menu_btn.pullUp(7, HIGH);
//	
//	for(int i=0; i<NR_MENU_BTN; i++){
//		mcp_menu_btn.pinMode(btn_menu_pins[i], INPUT);
//		mcp_menu_btn.pullUp(btn_menu_pins[i], HIGH);
//	}
//	bn_menu_btn.init_ArrBounce(btn_menu_pins, MENU_BTN_BOUNCE_TIME, NR_MENU_BTN, &btn_menu_digitalRead);
}

static void scan_menu_btn(){
	if(menu_btn.update()){
		if(menu_btn.fell()){
			current_prog = menu_ctrl.get_next_prog();
			lm_ptr = current_prog->get_led_matrix();
			menu_ctrl.menu_leave();
		} 
		else {
			menu_ctrl.menu_enter();
			lm_ptr = menu_ctrl.get_menu_led_matrix();
			current_prog = prog_arr[nr_prog];
		}
	} 
}