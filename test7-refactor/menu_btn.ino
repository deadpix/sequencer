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
#include <Bounce2.h>
//#include <Adafruit_MCP23017.h>
#include "src/Bounce_array.h"
#include "src/prog.h"
#include "src/param.h"

#define NR_MENU_BTN		4
#define MENU_BTN_BOUNCE_TIME	5
#define PIN_MENU_BTN		17
#define PIN_PARAM_BTN		16

#define FCT_BTN_IDLE		0
#define FCT_BTN_RELEASED	1
#define FCT_BTN_PUSHED		2
uint8_t param_btn_status;
uint8_t menu_btn_status;


Bounce menu_btn = Bounce();
Bounce param_btn = Bounce();
param* param_ptr;
//menu menu_ctrl;

//static uint8_t btn_menu_pins[NR_MENU_BTN] = {7, 6, 5, 4};
//Adafruit_MCP23017 mcp_menu_btn;

//static ArrBounce bn_menu_btn;

//static uint8_t btn_menu_digitalRead(uint8_t pin){
//	return mcp_menu_btn.digitalRead(pin);
//}

//void set_prog_menu_entry(uint8_t id, prog* prog){
//	menu_ctrl.set_menu_prog_entry(id, prog);
//}

static void init_menu_btn(prog* p){
//	menu_btn;
	param_btn_status = FCT_BTN_IDLE;
	menu_btn_status = FCT_BTN_IDLE;

	pinMode(PIN_MENU_BTN,INPUT);
	pinMode(PIN_PARAM_BTN,INPUT);
	menu_btn.attach(PIN_MENU_BTN);
	menu_btn.interval(MENU_BTN_BOUNCE_TIME);
	param_btn.attach(PIN_PARAM_BTN);
	param_btn.interval(MENU_BTN_BOUNCE_TIME);
	
	param_ptr = NULL;
	
	//	menu_ctrl.set_next_interface(p->get_led_matrix());
	sequenception.menu_ctrl.set_next_prog(p);
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
			menu_btn_status = FCT_BTN_RELEASED;
//			sequenception.current_prog = menu_ctrl.get_next_prog();
//			sequenception.current_prog->display_title();
//			lm_ptr = sequenception.current_prog->get_led_matrix();
//			menu_ctrl.menu_leave();
		} 
		else {
			menu_btn_status = FCT_BTN_PUSHED;
//			menu_ctrl.menu_enter();
//			lm_ptr = menu_ctrl.get_menu_led_matrix();
//			sequenception.current_prog = prog_arr[nr_prog];
		}
	}
	if(menu_btn_status == FCT_BTN_RELEASED){
		if(!flag_btn_active){
			sequenception.current_prog = sequenception.menu_ctrl.get_next_prog();
			sequenception.current_prog->display_title();
			sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
			sequenception.menu_ctrl.menu_leave();
			menu_btn_status = FCT_BTN_IDLE;
		}
	} else if(menu_btn_status == FCT_BTN_PUSHED){
		if(!flag_btn_active){
			sequenception.menu_ctrl.menu_enter();
			sequenception.lm_ptr = sequenception.menu_ctrl.get_menu_led_matrix();
			sequenception.current_prog = sequenception.prog_arr[sequenception.nr_prog];
			menu_btn_status = FCT_BTN_IDLE;
		}
	} 
}
static void scan_param_btn(){
	if(param_btn.update()){
		if(param_btn.fell() && param_ptr){
			param_btn_status = FCT_BTN_RELEASED; 
//			if(param_ptr){
//				sequenception.current_prog = param_ptr->get_prog();
//				lm_ptr = sequenception.current_prog->get_led_matrix();
//				param_ptr->param_on_leave();
//			}
		} 
		else {
			param_ptr = sequenception.current_prog->get_param();
			if(param_ptr){
				param_btn_status = FCT_BTN_PUSHED;
//				lm_ptr = param_ptr->get_led_matrix();
//				sequenception.current_prog = param_ptr;
//				param_ptr->param_on_enter();
//				//TODO print selected options
			}
		}
	}

	if(param_btn_status == FCT_BTN_RELEASED){
		if(!flag_btn_active){
			sequenception.current_prog = param_ptr->get_prog();
			sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
			param_ptr->param_on_leave();
			param_btn_status = FCT_BTN_IDLE;
		}	
	}
	else if(param_btn_status == FCT_BTN_PUSHED){
		if(!flag_btn_active){
			sequenception.lm_ptr = param_ptr->get_led_matrix();
			sequenception.current_prog = param_ptr;
			param_ptr->param_on_enter();
			param_btn_status = FCT_BTN_IDLE;
		}
	}
}
