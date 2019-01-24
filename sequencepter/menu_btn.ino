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

//#include <i2c_t3.h>
#include <Bounce2.h>
//#include <Adafruit_MCP23017.h>
#include "Bounce_array.h"
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

static void init_menu_btn(prog* p){
#if CMD_BTN_MATRIX == 0
	param_btn_status = FCT_BTN_IDLE;
	menu_btn_status = FCT_BTN_IDLE;

	pinMode(PIN_MENU_BTN,INPUT);
	pinMode(PIN_PARAM_BTN,INPUT);
	menu_btn.attach(PIN_MENU_BTN);
	menu_btn.interval(MENU_BTN_BOUNCE_TIME);
	param_btn.attach(PIN_PARAM_BTN);
	param_btn.interval(MENU_BTN_BOUNCE_TIME);
#endif	
	param_ptr = NULL;
	sequenception.menu_ctrl.set_next_prog(p);
}

int menu_on_push(){
	led_matrix* prev = sequenception.lm_ptr;
	int ret = 0;
	if(!flag_btn_active){
		sequenception.menu_ctrl.menu_enter();
		sequenception.lm_ptr = sequenception.menu_ctrl.get_menu_led_matrix();
		switch_matrix_ui(sequenception.lm_ptr, prev);
		sequenception.current_prog = sequenception.prog_arr[sequenception.nr_prog];
//		menu_btn_status = FCT_BTN_IDLE;
		ret = 1;
	}
	return ret;
}
int menu_on_release(){
	led_matrix* prev = sequenception.lm_ptr;
	int ret = 0;
	if(!flag_btn_active){
		sequenception.current_prog = sequenception.menu_ctrl.get_next_prog();
		sequenception.current_prog->display_title();
		sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
		switch_matrix_ui(sequenception.lm_ptr, prev);
		sequenception.menu_ctrl.menu_leave();
//		menu_btn_status = FCT_BTN_IDLE;
		ret = 1;
	}
	return ret;
}

int param_on_push(){
	led_matrix* prev = sequenception.lm_ptr;
	
	int ret = 0;
	if(!flag_btn_active){
		sequenception.current_prog = param_ptr;
		sequenception.lm_ptr = param_ptr->get_led_matrix();
		switch_matrix_ui(sequenception.lm_ptr, prev);
		param_ptr->param_on_enter();
//		param_btn_status = FCT_BTN_IDLE;
		ret = 1;
	}
	return ret;
}

int param_on_release(){
	led_matrix* prev = sequenception.lm_ptr;
	int ret = 0;
	if(!flag_btn_active){
		sequenception.current_prog = param_ptr->get_prog();
		sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
		switch_matrix_ui(sequenception.lm_ptr, prev);
		param_ptr->param_on_leave();
//		param_btn_status = FCT_BTN_IDLE;
		ret = 1;
	}
	return ret;

}

#if CMD_BTN_MATRIX == 0
static void scan_menu_btn(){
//	led_matrix* prev = sequenception.lm_ptr;
	if(menu_btn.update()){
		if(menu_btn.fell()){
			menu_btn_status = FCT_BTN_RELEASED;
		} 
		else {
			menu_btn_status = FCT_BTN_PUSHED;
		}
	}
	if(menu_btn_status == FCT_BTN_RELEASED){
		if(menu_on_release()){
			menu_btn_status = FCT_BTN_IDLE;			
		}
//		if(!flag_btn_active){
//			sequenception.current_prog = sequenception.menu_ctrl.get_next_prog();
//			sequenception.current_prog->display_title();
//			sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
//			switch_matrix_ui(sequenception.lm_ptr, prev);
//			sequenception.menu_ctrl.menu_leave();
//			menu_btn_status = FCT_BTN_IDLE;
//		}
	} else if(menu_btn_status == FCT_BTN_PUSHED){
		if(menu_on_push()){
			menu_btn_status = FCT_BTN_IDLE;
		}
//		if(!flag_btn_active){
//			sequenception.menu_ctrl.menu_enter();
//			sequenception.lm_ptr = sequenception.menu_ctrl.get_menu_led_matrix();
//			switch_matrix_ui(sequenception.lm_ptr, prev);
//			sequenception.current_prog = sequenception.prog_arr[sequenception.nr_prog];
//			menu_btn_status = FCT_BTN_IDLE;
//		}
	} 
}
static void scan_param_btn(){
	led_matrix* prev = sequenception.lm_ptr;
	if(param_btn.update()){
		if(param_btn.fell() && param_ptr){
			param_btn_status = FCT_BTN_RELEASED; 
		} 
		else {
			param_ptr = sequenception.current_prog->get_param();
			if(param_ptr){
				param_btn_status = FCT_BTN_PUSHED;
//				//TODO print selected options
			}
		}
	}

	if(param_btn_status == FCT_BTN_RELEASED){
		if(param_on_release()){
			param_btn_status = FCT_BTN_IDLE;
		}
//		if(!flag_btn_active){
//			sequenception.current_prog = param_ptr->get_prog();
//			sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
//			switch_matrix_ui(sequenception.lm_ptr, prev);
//			param_ptr->param_on_leave();
//			param_btn_status = FCT_BTN_IDLE;
//		}	
	}
	else if(param_btn_status == FCT_BTN_PUSHED){
		if(param_on_push()){
			param_btn_status = FCT_BTN_IDLE;			
		}
//		if(!flag_btn_active){
//			sequenception.current_prog = param_ptr;
//			sequenception.lm_ptr = param_ptr->get_led_matrix();
//			switch_matrix_ui(sequenception.lm_ptr, prev);
//			param_ptr->param_on_enter();
//			param_btn_status = FCT_BTN_IDLE;
//		}
	}
}
#endif
