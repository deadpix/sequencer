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

#include "led_matrix.h"
#include <hw_debug.h>
#include "bit.h"

//hw* led_matrix::_hw = NULL;

led_matrix::led_matrix(){
	for(int i=0;i<NR_LEDS;i++){
		for(int j=0;j<NR_LEVELS;j++){
			_led_status_arr[i].color[j] = 0;
		}
		_led_status_arr[i].bmp = 0x0;
	}
	_hw = NULL;
}

led_matrix::~led_matrix(){
}

struct led_status* led_matrix::get_led_status(uint16_t led_id){
	return &_led_status_arr[led_id];	
}

void led_matrix::set_hw(hw* h){
	_hw = h;
}
hw* led_matrix::get_hw(){
	return _hw;
}

int led_matrix::save_n_set(uint8_t color, uint16_t nr, uint8_t ground){
	int ret = 0;
		
	if(_led_status_arr[nr].bmp == 0){
		_led_status_arr[nr].bmp = (1 << ground);
		_led_status_arr[nr].color[ground] = color;
		ret = _led_status_arr[nr].bmp;
//		if(_hw) _hw->refresh_matrix(nr);
		if(_hw){
			_hw->upd_pxl(nr, color, ground);
			_hw->refresh_matrix(0);
		}
	}
	else if(BIT::is_bit_set(_led_status_arr[nr].bmp, ground)){
		ret = -1;
	} 
	else {
		uint8_t level_hi = BIT::get_highest_bit_set(_led_status_arr[nr].bmp);
		_led_status_arr[nr].bmp |= (1 << ground);
		_led_status_arr[nr].color[ground] = color;
		ret = _led_status_arr[nr].bmp;	
	
		if(level_hi < ground){
//			led_ovw(color, nr);
			if(_hw){
				_hw->upd_pxl(nr, color, ground);
				_hw->refresh_matrix(0);
			}
		}
	}
	return ret;	
}

int led_matrix::save_n_ovw(uint8_t color, uint16_t nr, uint8_t ground){
	int ret = 0;
		
	if(_led_status_arr[nr].bmp == 0){
		_led_status_arr[nr].bmp = (1 << ground);
		_led_status_arr[nr].color[ground] = color;
//		set_led_x(color, nr);
//		if(_hw) _hw->refresh_matrix(nr);
		if(_hw){
			_hw->upd_pxl(nr, color, ground);
			_hw->refresh_matrix(0);
		}
	}
	else if(BIT::is_bit_set(_led_status_arr[nr].bmp, ground)){
		_led_status_arr[nr].color[ground] = color;
		if(_hw){
	 		_hw->upd_pxl(nr, color, ground);
			_hw->refresh_matrix(0);
		}
//		led_ovw(color, nr);
	} 
	else {
		uint8_t level_hi = BIT::get_highest_bit_set(_led_status_arr[nr].bmp);
		_led_status_arr[nr].bmp |= (1 << ground);
		_led_status_arr[nr].color[ground] = color;

		if(level_hi < ground){
//			led_ovw(color, nr);
			if(_hw){
				_hw->upd_pxl(nr, color, ground);
				_hw->refresh_matrix(0);
			}
		}
	}

	return ret;	
}

int led_matrix::save_n_set_dfl(uint8_t color, uint16_t nr, uint8_t ground){
	int ret = 0;
	if(_led_status_arr[nr].bmp == 0){
		_led_status_arr[nr].bmp = (1 << ground);
		_led_status_arr[nr].color[ground] = color;
//		set_led_x(color, nr);
//		if(_hw) _hw->refresh_matrix(nr);
		if(_hw){
			_hw->upd_pxl(nr, color, ground);
			_hw->refresh_matrix(0);
		}
	} 
	else if(BIT::is_bit_set(_led_status_arr[nr].bmp, ground)){
		ret = -1;
	} 
	else {
		uint8_t level_hi = BIT::get_highest_bit_set(_led_status_arr[nr].bmp);
		_led_status_arr[nr].bmp |= (1 << ground);
		_led_status_arr[nr].color[ground] = color;
		ret = _led_status_arr[nr].bmp;	
	
		if(level_hi < ground){
//			_led_status_arr[nr].color[ground] = LED_OFF_IDX;
//			led_off(nr);
			if(_hw){
				_hw->upd_pxl(nr, color, ground);
				_hw->refresh_matrix(0);
			}
		}
//		if(_hw) _hw->refresh_matrix(nr);
	}
	return ret;
}

int led_matrix::save_n_toogle(uint8_t color, uint16_t nr, uint8_t ground){
	int ret = 0;
		
//	if(ground > 0){
//		ret = -1;
//	} 
//	else if(_led_status_arr[nr].bmp == 0){
//	else {
		_led_status_arr[nr].bmp |= (1 << ground);
		_led_status_arr[nr].color[ground] ^= color;
//		toogle_led_x(color, nr);
//		dbg::printf("save and toogle %d\n", _led_status_arr[nr].color[ground]);
//		if(_hw) _hw->refresh_matrix(nr);
		if(_hw){
			_hw->upd_pxl(nr, _led_status_arr[nr].color[ground], ground);
			_hw->refresh_matrix(0);
		}
//	}
	return ret;
	
}
int led_matrix::clr_n_restore(uint16_t nr, uint8_t ground){
	int ret = 0;
//	clr_led_x(_led_status_arr[nr].color[ground], nr);
	_led_status_arr[nr].color[ground] = 0;
	_led_status_arr[nr].bmp &= ~(1<<ground);

	if( (ret = _led_status_arr[nr].bmp) ){
		uint8_t level_hi = BIT::get_highest_bit_set(_led_status_arr[nr].bmp);
//		set_led_x(_led_status_arr[nr].color[level_hi], nr);
		if(_hw){
			_hw->upd_pxl(nr, _led_status_arr[nr].color[level_hi], level_hi);
			_hw->refresh_matrix(0);
		}
	} 
	else {
		if(_hw){
			_hw->upd_pxl(nr, 0x0, ground);
			_hw->refresh_matrix(0);
		}
	}
//	if(_hw) _hw->refresh_matrix(nr);
	return  _led_status_arr[nr].bmp;
//	return ret;	
}
