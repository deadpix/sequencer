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

#include "btn_state.h"

btn_state::btn_state(){
	for(int i=0;i<BTN_MATRIX_COL;i++){
		btn_arr[i].bitmap = 0x0;
	}
}
btn_state::~btn_state(){
}


int btn_state::set_btn_active_coor(uint8_t x, uint8_t y){
	int res = 0;
	btn_arr[x].bitmap |= (1<<y);


	return res;
}

int btn_state::clr_btn_active_coor(uint8_t x, uint8_t y){
	int res = 0;
	btn_arr[x].bitmap &= ~(1<<y);

	
	return res;
}

int btn_state::set_btn_active(uint8_t id){
	uint16_t x = id / BTN_MATRIX_ROW;	
	uint16_t y = id % BTN_MATRIX_ROW;	
	
	return set_btn_active_coor(x, y);

}

int btn_state::clr_btn_active(uint8_t id){
	uint16_t x = id / BTN_MATRIX_ROW;	
	uint16_t y = id % BTN_MATRIX_ROW;	

	return clr_btn_active_coor(x, y);
}

