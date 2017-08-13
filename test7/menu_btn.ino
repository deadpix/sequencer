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

#define NR_MENU_BTN				4
#define MENU_BTN_BOUNCE_TIME	5

static uint8_t btn_menu_pins[NR_MENU_BTN] = { 0, 1, 2, 3};
Adafruit_MCP23017 mcp_menu_btn;

static ArrBounce menu_btn;

static uint8_t btn_menu_digitalRead(uint8_t pin){
	return mcp_menu_btn.digitalRead(pin);
}

static void init_menu_btn(){
	menu_btn.init_ArrBounce(btn_menu_pins, MENU_BTN_BOUNCE_TIME, NR_MENU_BTN, &btn_menu_digitalRead);
}

static void scan_menu_btn(){
	int i = 0;
	for (i=0; i<NR_MENU_BTN; i++){
		if(menu_btn.update(i)){
			if(menu_btn.read(i) == LOW){
				Serial.println("pushed");
			} else {
				Serial.println("released");				
			}		
		}
	}
}