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

#include "hw_nt.h"
#include <hw_debug.h>

// sequenception to neotrellis color
//static uint32_t sqcpt_to_nt_color[NR_LEVELS][NR_COLORS]; 
hw_nt::hw_nt(Adafruit_MultiTrellis* mt){
	_mt = mt;
	_sqcpt_to_nt_color[0][0] = seesaw_NeoPixel::Color(0, 0, 0);
	_sqcpt_to_nt_color[0][1] = seesaw_NeoPixel::Color(255, 0, 0);
	_sqcpt_to_nt_color[0][2] = seesaw_NeoPixel::Color(0, 255, 0);
	_sqcpt_to_nt_color[0][3] = seesaw_NeoPixel::Color(255, 255, 0);
	_sqcpt_to_nt_color[0][4] = seesaw_NeoPixel::Color(0, 0, 255);
	_sqcpt_to_nt_color[0][5] = seesaw_NeoPixel::Color(255, 0, 255);
	_sqcpt_to_nt_color[0][6] = seesaw_NeoPixel::Color(0, 255, 255);
	_sqcpt_to_nt_color[0][7] = seesaw_NeoPixel::Color(255, 255, 255);

	_sqcpt_to_nt_color[1][0] = seesaw_NeoPixel::Color(0, 0, 0);
	_sqcpt_to_nt_color[1][1] = seesaw_NeoPixel::Color(255, 100, 100);
	_sqcpt_to_nt_color[1][2] = seesaw_NeoPixel::Color(100, 255, 100);
	_sqcpt_to_nt_color[1][3] = seesaw_NeoPixel::Color(255, 255, 100);
	_sqcpt_to_nt_color[1][4] = seesaw_NeoPixel::Color(100, 100, 255);
	_sqcpt_to_nt_color[1][5] = seesaw_NeoPixel::Color(255, 100, 255);
	_sqcpt_to_nt_color[1][6] = seesaw_NeoPixel::Color(100, 255, 255);
	_sqcpt_to_nt_color[1][7] = seesaw_NeoPixel::Color(255, 255, 255);

	_sqcpt_to_nt_color[2][0] = seesaw_NeoPixel::Color(0, 0, 0);
	_sqcpt_to_nt_color[2][1] = seesaw_NeoPixel::Color(255, 200, 200);
	_sqcpt_to_nt_color[2][2] = seesaw_NeoPixel::Color(200, 255, 200);
	_sqcpt_to_nt_color[2][3] = seesaw_NeoPixel::Color(255, 255, 200);
	_sqcpt_to_nt_color[2][4] = seesaw_NeoPixel::Color(200, 200, 255);
	_sqcpt_to_nt_color[2][5] = seesaw_NeoPixel::Color(255, 200, 255);
	_sqcpt_to_nt_color[2][6] = seesaw_NeoPixel::Color(200, 255, 255);
	_sqcpt_to_nt_color[2][7] = seesaw_NeoPixel::Color(255, 255, 255);
}

hw_nt::~hw_nt(){
};

void hw_nt::refresh_matrix(uint16_t id){
	_mt->show();
}

void hw_nt::upd_pxl(uint16_t id, uint8_t color, uint8_t brightness){
/*
	dbg::print("passe id");		
	dbg::print(id);
	dbg::print(" color ");		
	dbg::print(color);
	dbg::print(" brightness ");
	dbg::print(brightness);
	dbg::print(" rgb ");
	dbg::println(_sqcpt_to_nt_color[brightness][color]);
*/
	_mt->setPixelColor(id, _sqcpt_to_nt_color[brightness][color]);
}
