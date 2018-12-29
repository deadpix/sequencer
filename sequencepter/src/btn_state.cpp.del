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
#include "bit.h"

btn_state::btn_state(){
	for(uint8_t i=0;i<MATRIX_NR_COL;i++){
		short_push[i] = 0x0;
		long_push[i] = 0x0;
	}
}

void btn_state::set_short_push(uint8_t id){
	short_push[id / MATRIX_NR_COL] |= (1 << MATRIX_NR_COL);
}
void btn_state::clr_short_push(uint8_t id){
	short_push[id / MATRIX_NR_COL] &= ~(1 << MATRIX_NR_COL);
}
void btn_state::set_long_push(uint8_t id){
	long_push[id / MATRIX_NR_COL] |= (1 << MATRIX_NR_COL);
}
void btn_state::clr_long_push(uint8_t id){
	long_push[id / MATRIX_NR_COL] &= ~(1 << MATRIX_NR_COL);
}

uint8_t btn_state::get_nr_short_push(){
	uint8_t bit = 0, cnt = 0;
	for(uint8_t i=0; i<MATRIX_NR_COL; i++){
		for_eachset_bit(bit, (uint16_t *) &short_push[i], BIT_PER_BYTE){
			cnt++;
		}
	}
	return cnt;
}
uint8_t btn_state::get_nr_long_push(){
	uint8_t bit = 0, cnt = 0;
	for(uint8_t i=0;i<MATRIX_NR_COL;i++){
		for_eachset_bit(bit, (uint16_t *) &long_push[i], BIT_PER_BYTE){
			cnt++;
		}
	}
	return cnt;

}
uint8_t btn_state::get_short_push_id(uint8_t* arr){
	uint8_t bit = 0, cnt = 0, arr_size = sizeof(arr) / sizeof(arr[0]);
	for(uint8_t i=0;i<MATRIX_NR_COL;i++){
		for_eachset_bit(bit, (uint16_t *) &short_push[i], BIT_PER_BYTE){
			arr[++cnt] = bit;
			if(cnt == arr_size){
				return cnt;
			}
		}
	}
	return cnt;

}
uint8_t btn_state::get_long_push_id(uint8_t* arr){
	uint8_t bit = 0, cnt = 0, arr_size = sizeof(arr) / sizeof(arr[0]);
	for(uint8_t i=0;i<MATRIX_NR_COL;i++){
		for_eachset_bit(bit, (uint16_t *) &long_push[i], BIT_PER_BYTE){
			arr[++cnt] = bit;
			if(cnt == arr_size){
				return cnt;
			}
		}
	}
	return cnt;
}
