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

#include "hw_lcd_ILI9341.h"
#include <hw_debug.h>
#include "../perf.h"

#define TFT_X_OFFSET			16
#define TFT_Y_OFFSET			56
#define TFT_MATRIX_PIX_SIZE		208
#define TFT_CELL_PIX_SIZE		26
#define TFT_CELL_PIX_OFFSET		2
#define TFT_ELEMENT_PIX_SIZE	(TFT_CELL_PIX_SIZE - 2*TFT_CELL_PIX_OFFSET)

//#define DEBUG

void hw_lcd_ILI9341::resetLcd(){
	_tft->fillScreen(ILI9341_BLACK);
	uint16_t x_start = TFT_X_OFFSET;
	uint16_t x_end = TFT_MATRIX_PIX_SIZE+TFT_X_OFFSET;

	uint16_t y_start = TFT_Y_OFFSET;
	uint16_t y_end = TFT_MATRIX_PIX_SIZE+TFT_Y_OFFSET;

	for(int i=0; i<MATRIX_NR_COL+1; i++){
		int inc = (TFT_CELL_PIX_SIZE * i);
		_tft->drawLine(x_start, y_start + inc , x_end, y_start + inc, ILI9341_WHITE);
		_tft->drawLine(x_start+inc, y_start, x_start + inc, y_end, ILI9341_WHITE);
	}
}

hw_lcd_ILI9341::hw_lcd_ILI9341(ILI9341_t3* tft){
	_tft = tft;
	for(int i=0;i<3; i++){
		_sqcpt_to_lcd_color[i][0] = ILI9341_BLACK;
		_sqcpt_to_lcd_color[i][1] = ILI9341_RED;
		_sqcpt_to_lcd_color[i][2] = ILI9341_BLUE;
		_sqcpt_to_lcd_color[i][3] = ILI9341_MAGENTA;
		_sqcpt_to_lcd_color[i][4] = ILI9341_GREEN;
		_sqcpt_to_lcd_color[i][5] = ILI9341_YELLOW;
		_sqcpt_to_lcd_color[i][6] = ILI9341_CYAN;
		_sqcpt_to_lcd_color[i][7] = ILI9341_WHITE;		
	}
}

void hw_lcd_ILI9341::upd_pxl(uint16_t id, uint8_t color, uint8_t brightness){
	// draw pixel with color _sqcpt_to_lcd_color[brightness][color] 
	int x 	= TFT_X_OFFSET + TFT_CELL_PIX_OFFSET 
			+ (id % MATRIX_NR_COL) * TFT_CELL_PIX_SIZE;
	int y 	= TFT_Y_OFFSET + TFT_CELL_PIX_OFFSET
			+ (id / MATRIX_NR_COL) * TFT_CELL_PIX_SIZE;

#ifdef DEBUG
	dbg::print("id: ");
	dbg::print(id);
	dbg::print(" x=");
	dbg::print(x);
	dbg::print(" y=");
	dbg::println(y);
#endif

	_tft->fillRect(x, y, TFT_ELEMENT_PIX_SIZE, TFT_ELEMENT_PIX_SIZE, 
				  _sqcpt_to_lcd_color[brightness][color]);

}