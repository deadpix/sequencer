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


#ifndef __SRC_HW_HW_LCD_ILI9341_H__
#define __SRC_HW_HW_LCD_ILI9341_H__

#include "../hw.h"
#include "../types.h" 
#include <ILI9341_t3.h>


#define NR_COLORS	8
class hw_lcd_ILI9341 : public hw {
	private:
		ILI9341_t3* _tft;
		uint32_t _sqcpt_to_lcd_color[3][NR_COLORS];

	public:
		hw_lcd_ILI9341(ILI9341_t3* tft);
		~hw_lcd_ILI9341(){};
		led_t get_led(uint8_t){};
		void refresh_matrix(uint16_t id) {};
		void upd_pxl(uint16_t id, uint8_t color, uint8_t brightness);
		void resetLcd();
};


#endif