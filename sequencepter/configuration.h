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


#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#define PROTO1 	1

#if PROTO1 == 1

#define HW_SHIFT_REGISTER		0
#define HW_ADAFRUIT_NEOTRELLIS	0
#define HW_SPARKFUN_LUMINI		0
#define HW_LCD_ILI9341			1
#define CMD_BTN_MATRIX  		0



#else

#define HW_SHIFT_REGISTER		0
#define HW_ADAFRUIT_NEOTRELLIS	0
#define HW_SPARKFUN_LUMINI		0
#define HW_LCD_ILI9341			1
#define CMD_BTN_MATRIX  		0

#endif

#endif
