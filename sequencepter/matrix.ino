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
//#include <Adafruit_MCP23017.h>

#include "configuration.h"
#include "src/types.h"
#include "src/hw.h"
#include "src/led_matrix.h"
#include "src/bit.h"
#include "Bounce_array.h"
#include "PinInterface.h"

#define BTN_NUM_COL		8
#define BTN_NUM_ROW		8
#define BOUNCE_TIME		5

#define LONG_PRESS_TIME_MS	1000
#define NR_CBCK			3

class CpuPinReader: public PinInterface {
	public:
//		CpuPinReader(){};
//		~CpuPinReader(){};
		uint8_t readPin(uint8_t pin){ return digitalRead(pin); }
};

class McpPinReader: public PinInterface {
private:
	Adafruit_MCP23017* _mcp;

public:
//	McpPinReader(){};
//	~McpPinReader(){};
	void attachMcp(Adafruit_MCP23017* mcp){ _mcp = mcp; }
	uint8_t readPin(uint8_t pin){ return _mcp->digitalRead(pin); }
};

CpuPinReader cpuPinReader;

Adafruit_MCP23017 mcp;
McpPinReader	  mcpPinReader0;

#if CMD_BTN_MATRIX == 1 
Adafruit_MCP23017 mcp1;
McpPinReader	  mcpPinReader1;
#endif

struct _btn_status {
	byte pushed_bmp[BTN_NUM_COL];
	byte long_pushed_bmp[BTN_NUM_COL];
};
static struct _btn_status btn_status;


cbck_fct_rd cbck_fct_rd_arr[NR_CBCK];

int init_rd_cbck(cbck_fct_rd fct_ptr, uint8_t fct_id){
	int res = 1;
	if(fct_id < NR_CBCK){
		res = 0;
		cbck_fct_rd_arr[fct_id] = fct_ptr;
	}
	return res;
}
uint8_t mcp_digitalRead(uint8_t pin, uint8_t fct_id){
	return cbck_fct_rd_arr[fct_id](pin);
}



#if CMD_BTN_MATRIX == 1

#define CMD_BTN_MATRIX_CLBK	1
#define MCP1_ADDR		0x0
#define CMD_BTN_MATRIX_NR_ROW	8
#define CMD_BTN_MATRIX_NR_COL	8

//#if CMD_BTN_MATRIX_NR_ROW <= 8
typedef uint8_t cmd_btn_status_bmp_t;
//#elif CMD_BTN_MATRIX_NR_ROW <= 16
//typedef uint16_t cmd_btn_status_bmp_t;
//#elif CMD_BTN_MATRIX_NR_ROW <= 32
//typedef uint32_t cmd_btn_status_bmp_t;
//#endif
#define CMD_BTN_IS_PUSHED	1
#define CMD_BTN_IS_RELEASED	0
cmd_btn_status_bmp_t cmd_btn_action_bmp[CMD_BTN_MATRIX_NR_COL];
cmd_btn_status_bmp_t cmd_btn_status_bmp[CMD_BTN_MATRIX_NR_COL];


static uint8_t cmd_btn_matrix_select_pins[CMD_BTN_MATRIX_NR_COL] = {0,1,2,3,4,5,6,7}; // ground switch
static uint8_t cmd_btn_matrix_read_pins[CMD_BTN_MATRIX_NR_ROW] = {8,9,10,11,12,13,14,15};

static ArrBounce cmd_btn_matrix_status[CMD_BTN_MATRIX_NR_ROW];
static uint8_t cmd_btn_matrix_col_cnt;

static uint8_t cmd_btn_matrix_digitalRead(uint8_t pin){
	return mcp1.digitalRead(pin);
}

static void setup_cmd_btn_matrix(){
	int i = 0;
	cmd_btn_matrix_col_cnt = 0;

	mcp1.begin(MCP1_ADDR);
	Wire.setClock(I2C_RATE_800);

	if(init_rd_cbck(&cmd_btn_matrix_digitalRead, CMD_BTN_MATRIX_CLBK)){
		Serial.println("Could not init MCP23017 callback function array...");
	}

	for (i=0;i<CMD_BTN_MATRIX_NR_COL;i++){
		cmd_btn_action_bmp[i] = 0x0;
		cmd_btn_status_bmp[i] = 0x0;
        cmd_btn_matrix_status[i].init_ArrBounce(cmd_btn_matrix_read_pins, 
        										BOUNCE_TIME, 
        										CMD_BTN_MATRIX_NR_ROW, 
        										(PinInterface *) &mcpPinReader1);
		mcp1.pinMode(cmd_btn_matrix_select_pins[i], OUTPUT);
		mcp1.digitalWrite(cmd_btn_matrix_select_pins[i], HIGH);
	}

	// button row input lines
	for (i=0;i<CMD_BTN_MATRIX_NR_ROW;i++){
		mcp1.pinMode(cmd_btn_matrix_read_pins[i], INPUT);
		mcp1.pullUp(cmd_btn_matrix_read_pins[i], HIGH);
	}

}
static int cmd_btn_matrix_do_something(uint8_t cmd_btn, bool is_pushed){
	int ret = 0;
	switch(cmd_btn){
		case 0:
			if(is_pushed){
				ret = menu_on_push();	
			} else {
				ret = menu_on_release();
			}
			break;

		case 1:
			if(is_pushed){
				if( (param_ptr = sequenception.current_prog->get_param()) ){
					ret = param_on_push();	
				}
				else {
					ret = 1;
				}
			} else {
				if(param_ptr){
					ret = param_on_release();
				}
				else { 
					ret = 1;
				}
			}
			break;

		default:
			Serial.print("btn ");
			Serial.print(cmd_btn);
			if(is_pushed){
				Serial.println(" has no pushed cmd");
			} 
			else {
				Serial.println(" has no released cmd");
			}
			ret = 1;
			break;
	}
	return ret;
}

static void cmd_btn_matrix_action(uint8_t col_cnt){
	int idx;
	uint16_t *action_bmp = (uint16_t *) &cmd_btn_action_bmp[col_cnt];
	uint16_t *status_bmp = (uint16_t *) &cmd_btn_status_bmp[col_cnt];
	if(action_bmp){
//		uint16_t bmp = (uint16_t) cmd_btn_action_bmp[col_cnt];
		for_eachset_bit(idx, action_bmp, CMD_BTN_MATRIX_NR_ROW){
			if(cmd_btn_matrix_do_something(
				col_cnt*CMD_BTN_MATRIX_NR_COL+idx, 
				BIT::is_bit_set(*status_bmp, idx))
			){
				BIT::clear_bits(action_bmp, idx, 1);
			}
		}
	}
}

static void scan_cmd_btn_matrix(){
	int i = 0;
//	if(cmd_btn_matrix_col_cnt == 0)
//		Serial.println("scan_cmd_btn_matrix");


	mcp1.digitalWrite(cmd_btn_matrix_select_pins[cmd_btn_matrix_col_cnt], LOW);
	
	for (i=0; i<CMD_BTN_MATRIX_NR_COL; i++){
		if(cmd_btn_matrix_status[cmd_btn_matrix_col_cnt].update(i)){
			BIT::set_bits((uint16_t *) &cmd_btn_action_bmp[cmd_btn_matrix_col_cnt], i, 1);
			if(cmd_btn_matrix_status[cmd_btn_matrix_col_cnt].read(i) == HIGH){
				// released		
//				cmd_btn_matrix_action(cmd_btn_matrix_col_cnt*CMD_BTN_MATRIX_NR_COL + i, false);
				BIT::clear_bits((uint16_t *) &cmd_btn_status_bmp[cmd_btn_matrix_col_cnt], i, 1);
			} else {
				// pushed
//				cmd_btn_matrix_action(cmd_btn_matrix_col_cnt*CMD_BTN_MATRIX_NR_COL + i, true);
				BIT::set_bits((uint16_t *) &cmd_btn_status_bmp[cmd_btn_matrix_col_cnt], i, 1);
				
			}
		}
	}
	
	mcp1.digitalWrite(cmd_btn_matrix_select_pins[cmd_btn_matrix_col_cnt], HIGH);	
	cmd_btn_matrix_col_cnt = (cmd_btn_matrix_col_cnt+1) % CMD_BTN_MATRIX_NR_COL;


	cmd_btn_matrix_action(cmd_btn_matrix_col_cnt);
}



#endif



static elapsedMillis btn_ms_cnt[NR_LEDS];

#if HW_SHIFT_REGISTER == 1

#include "src/hw/hw_sr.h"
#define latchPin 		22
#define clockPin 		23
#define dataPin  		21

#define GRD_OFFSET		0
#define BLUE_OFFSET		16
#define GREEN_OFFSET		24
#define RED_OFFSET		8

static hw_sr hw;

static uint8_t btn_select_pins[BTN_NUM_COL] = {0,1,2,3,4,5,6,7}; // ground switch
static uint8_t btn_read_pins[BTN_NUM_ROW] = {8,9,10,11,12,13,14,15};

//static uint8_t btn_select_pins[BTN_NUM_COL] 	= { 7, 6, 5, 4, 3, 2, 1, 0};
//static uint8_t btn_read_pins[BTN_NUM_ROW] 	= {12, 13, 14, 15, 8, 9, 10, 11 };
static ArrBounce btn_row[BTN_NUM_COL];
static uint8_t grd_cnt;

static uint8_t btn_matrix_digitalRead(uint8_t pin){
	return mcp.digitalRead(pin);
}

static void setup_matrix(){
	int i;
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);

	write_shift_reg(0x0);
	grd_cnt = 0;

	btn_col_idx = 0;
	flag_btn_active = false;

	init_rd_cbck(&btn_matrix_digitalRead, 0);
	mcp.begin(0x0);
	Wire.setClock(1000000);

	for(i=0;i<BTN_NUM_COL;i++){
		btn_row[i].init_ArrBounce(btn_read_pins, BOUNCE_TIME, BTN_NUM_ROW, (PinInterface *) &mcpPinReader0);
		btn_status.pushed_bmp[i] = 0x0;
		btn_status.long_pushed_bmp[i] = 0x0;
	}

	for (i=0;i<BTN_NUM_COL;i++){
		mcp.pinMode(btn_select_pins[i], OUTPUT);
		mcp.digitalWrite(btn_select_pins[i], HIGH);
	}

	// button row input lines
	for (i=0;i<BTN_NUM_ROW;i++){
		mcp.pinMode(btn_read_pins[i], INPUT);
		mcp.pullUp(btn_read_pins[i], HIGH);
	}
}

static void scan(prog* p){
	uint8_t i;

// 	Select current columns
	mcp.digitalWrite(btn_select_pins[btn_col_idx], LOW);


// 	Read the button inputs
	for (i=0; i<BTN_NUM_ROW; i++){
		if(btn_row[btn_col_idx].update(i)){
			if(btn_row[btn_col_idx].read(i) == HIGH){
//				p->on_release(btn_col_idx*BTN_NUM_COL + i);
				flag_btn_active = false;
				if(btn_status.long_pushed_bmp[btn_col_idx] & (1<<i)){
					// release
					p->on_long_release(btn_col_idx*BTN_NUM_COL + i);
				} 
				else {
					p->on_release(btn_col_idx*BTN_NUM_COL + i);
				}
				btn_status.pushed_bmp[btn_col_idx] &= ~(1<<i);
				btn_status.long_pushed_bmp[btn_col_idx] &= ~(1<<i);
				
			} else {
				p->on_push(btn_col_idx*BTN_NUM_COL + i);
				flag_btn_active = true;
				btn_ms_cnt[btn_col_idx*BTN_NUM_COL + i] = 0;
				btn_status.pushed_bmp[btn_col_idx] |= (1<<i);
			}
		}
		if( (btn_status.pushed_bmp[btn_col_idx] & (1<<i)) 
		&& !(btn_status.long_pushed_bmp[btn_col_idx] & (1<<i))){
			if(btn_ms_cnt[btn_col_idx*BTN_NUM_COL + i] > LONG_PRESS_TIME_MS){
				btn_status.long_pushed_bmp[btn_col_idx] |= (1<<i);
				p->on_long_push(btn_col_idx*BTN_NUM_COL + i);
			}
		}
	}
	mcp.digitalWrite(btn_select_pins[btn_col_idx], HIGH);	
	btn_col_idx = (btn_col_idx+1)%BTN_NUM_COL;
}
static void write_shift_reg(uint32_t val){
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, val);
	shiftOut(dataPin, clockPin, MSBFIRST, val >> 8);
	shiftOut(dataPin, clockPin, MSBFIRST, val >> 16);
	shiftOut(dataPin, clockPin, MSBFIRST, val >> 24);
	digitalWrite(latchPin, HIGH);
}

static void upd_shift_reg(led_matrix* lm){
	uint32_t tmp = ((1<<(grd_cnt/*+4*/))) << GRD_OFFSET;
	led_t l = lm->get_hw()->get_led(grd_cnt);	
	tmp |= (~l.bitmap[LED_COLOR_RED_INDEX] << RED_OFFSET)
		 | (~l.bitmap[LED_COLOR_GREEN_INDEX] << GREEN_OFFSET)
		 | (~l.bitmap[LED_COLOR_BLUE_INDEX] << BLUE_OFFSET);

//`	Serial.println(tmp);
	
	write_shift_reg(tmp);
	grd_cnt = (grd_cnt + 1) % LED_MATRIX_NR_GROUND;
}
static void switch_matrix_ui(led_matrix* next, led_matrix* prev){
	prev->set_hw(NULL);
	next->set_hw(&hw);

	for(int i=0; i<NR_LEDS; i++){
		struct led_status* tmp = next->get_led_status(i);
		if(tmp->bmp){
			uint8_t idx = BIT::get_highest_bit_set(tmp->bmp);
			hw.upd_pxl(i, tmp->color[idx], idx);
		}
		else {
			hw.upd_pxl(i, 0, 0);
		}
		hw.refresh_matrix(0);
	}
	
}

#if 0
#elif HW_ADAFRUIT_NEOTRELLIS == 1

#include <Adafruit_NeoTrellis.h>
#include "src/hw/hw_nt.h"
#include "src/bit.h"
#define NT_INT_PIN	17


#define NT_ADDR1 0x2F
#define NT_ADDR2 0x30
#define NT_ADDR3 0x36
#define NT_ADDR4 0x2E

//create a matrix of trellis panels
Adafruit_NeoTrellis t_array[BTN_NUM_ROW/4][BTN_NUM_COL/4] = {
  
 // { Adafruit_NeoTrellis(NT_ADDR1, &Wire), Adafruit_NeoTrellis(NT_ADDR2, &Wire) },
 // { Adafruit_NeoTrellis(NT_ADDR3, &Wire), Adafruit_NeoTrellis(NT_ADDR4, &Wire) }
   
//  { Adafruit_NeoTrellis(NT_ADDR1, &Wire1), Adafruit_NeoTrellis(NT_ADDR2, &Wire1) },
//  { Adafruit_NeoTrellis(NT_ADDR3, &Wire1), Adafruit_NeoTrellis(NT_ADDR4, &Wire1) }

  { Adafruit_NeoTrellis(NT_ADDR1), Adafruit_NeoTrellis(NT_ADDR2) },
  { Adafruit_NeoTrellis(NT_ADDR3), Adafruit_NeoTrellis(NT_ADDR4) }


};

Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, BTN_NUM_ROW/4, BTN_NUM_COL/4);

static hw_nt hw(&trellis);

TrellisCallback trellis_btn_clbck(keyEvent evt){
//	Serial.print("Triggered event ");
//	Serial.println(evt.bit.NUM);
	prog* p = sequenception.current_prog;
	uint8_t row = evt.bit.NUM / BTN_NUM_ROW;	
	uint8_t offset = evt.bit.NUM % BTN_NUM_ROW;

	if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
		flag_btn_active = true;
		Serial.print("push ");
		Serial.println(evt.bit.NUM);
		p->on_push(evt.bit.NUM);
		btn_ms_cnt[evt.bit.NUM] = 0;
		btn_status.pushed_bmp[row] |= (1<<offset);	
	}
	else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
		flag_btn_active = false;
		Serial.print("release ");
		Serial.println(evt.bit.NUM);

//		if(btn_ms_cnt[evt.bit.NUM] > LONG_PRESS_TIME_MS){
		if(btn_status.long_pushed_bmp[row] & (1<<offset)){
			// release
//			p->on_long_push(evt.bit.NUM);
			p->on_long_release(evt.bit.NUM);
		} 
		else {
			p->on_release(evt.bit.NUM);
		}
		btn_status.pushed_bmp[row] &= ~(1<<offset);
		btn_status.long_pushed_bmp[row] &= ~(1<<offset);

	}
//	if( (btn_status.pushed_bmp[row] & (1<<offset)) 
//	&& !(btn_status.long_pushed_bmp[row] & (1<<offset))){
//		if(btn_ms_cnt[evt.bit.NUM] > LONG_PRESS_TIME_MS){
//			btn_status.long_pushed_bmp[row] |= (1<<offset);
//			p->on_long_push(evt.bit.NUM);
//		}
//	}

	return 0;
}

static void setup_matrix(){
	if(!trellis.begin()){
		Serial.println("failed to begin trellis");
		return;
//		while(1);
	}

	flag_btn_active = false;
//	pinMode(NT_INT_PIN, INPUT);

  	for(int y=0; y<BTN_NUM_ROW; y++){
		for(int x=0; x<BTN_NUM_COL; x++){
			// activate rising and falling edges on all keys
			trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
			trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
			trellis.registerCallback(x, y, trellis_btn_clbck);
		//	trellis.setPixelColor(x, y, 0x000000); //addressed with x,y
			trellis.setPixelColor(x, y, 0x0000AA); //addressed with x,y
			trellis.show(); //show all LEDs
    		}
		btn_status.pushed_bmp[y] = 0x0;
		btn_status.long_pushed_bmp[y] = 0x0;
		delay(50);
	}
}
static void scan(prog* p){
//	if(!digitalRead(NT_INT_PIN)){
//		trellis.read(false);
//	}
	trellis.read();

	for(int row = 0; row < BTN_NUM_ROW; row++){
		for (uint8_t offset=0; offset<BTN_NUM_COL;offset++){
			uint8_t idx = row * 8 + offset;
			if( (btn_status.pushed_bmp[row] & (1<<offset)) 
			&& !(btn_status.long_pushed_bmp[row] & (1<<offset))){
				if(btn_ms_cnt[idx] > LONG_PRESS_TIME_MS){
					btn_status.long_pushed_bmp[row] |= (1<<offset);
					p->on_long_push(idx);
				}
			}
		}
	}
}
static void upd_shift_reg(led_matrix* lm){
}
static void switch_matrix_ui(led_matrix* next, led_matrix* prev){
	prev->set_hw(NULL);
	next->set_hw(&hw);

	for(int i=0; i<NR_LEDS; i++){
		struct led_status* tmp = next->get_led_status(i);
		if(tmp->bmp){
			uint8_t idx = BIT::get_highest_bit_set(tmp->bmp);
			hw.upd_pxl(i, tmp->color[idx], idx);
		}
		else {
			hw.upd_pxl(i, 0, 0);
		}
		hw.refresh_matrix(0);
	}
}
#endif
/*
#elif HW_SPARKFUN_LUMINI == 1
#include "src/hw/hw_lu.h"
#include <FastLED.h>

#define DATA_PIN 	11
#define CLOCK_PIN 	14

CRGB matrix[NR_LEDS];
static hw_lu hw(matrix);

static void scan(prog* p){
	// TODO
	Serial.println("TODO");
}
static void upd_shift_reg(led_matrix* lm){
	// EMPTY
}
static void switch_matrix_ui(led_matrix* next, led_matrix* prev){
	prev->set_hw(NULL);
	next->set_hw(&hw);

	for(int i=0; i<NR_LEDS; i++){
		struct led_status* tmp = next->get_led_status(i);
		if(tmp->bmp){
			uint8_t idx = BIT::get_highest_bit_set(tmp->bmp);
			hw.upd_pxl(i, tmp->color[idx], idx);
		}
		else {
			hw.upd_pxl(i, 0, 0);
		}
	}
	hw.refresh_matrix(0);
}

static void setup_matrix(){
	
	Serial.println("setup lumini matrix");
	
	LEDS.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(matrix, NR_LEDS);
	LEDS.setBrightness(20);

	delay(1000);
}
*/
#elif HW_LCD_ILI9341 == 1

#include <SPI.h>
#include <ILI9341_t3.h>
#include "src/hw/hw_lcd_ILI9341.h"

#define TFT_DC  9
#define TFT_CS 10
#define MCP1_ADDR		0x0
Adafruit_MCP23017 mcp1;
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
static hw_lcd_ILI9341 hw(&tft);

static uint8_t btn_select_pins[BTN_NUM_COL] = { A13, 24, 25, 26, 31, 32, 33, A11 };
static uint8_t btn_read_pins[BTN_NUM_ROW] 	= { 23, 22, 28, 27, 24, 23, 22, 21};

static ArrBounce btn_row[BTN_NUM_COL];
static uint8_t grd_cnt;


static uint8_t btn_matrix_digitalRead(uint8_t pin){
	return digitalRead(pin);
}

static void scan(prog* p){
	uint8_t i;

	digitalWrite(btn_select_pins[btn_col_idx], LOW);

	for (i=0; i<BTN_NUM_ROW; i++){
		if(btn_row[btn_col_idx].update(i)){
			if(btn_row[btn_col_idx].read(i) == HIGH){
				flag_btn_active = false;
				if(btn_status.long_pushed_bmp[btn_col_idx] & (1<<i)){
					// release
					p->on_long_release(btn_col_idx*BTN_NUM_COL + i);
				} 
				else {
					p->on_release(btn_col_idx*BTN_NUM_COL + i);
				}
				btn_status.pushed_bmp[btn_col_idx] &= ~(1<<i);
				btn_status.long_pushed_bmp[btn_col_idx] &= ~(1<<i);
				
			} else {
				p->on_push(btn_col_idx*BTN_NUM_COL + i);
				flag_btn_active = true;
				btn_ms_cnt[btn_col_idx*BTN_NUM_COL + i] = 0;
				btn_status.pushed_bmp[btn_col_idx] |= (1<<i);
			}
		}
		if( (btn_status.pushed_bmp[btn_col_idx] & (1<<i)) 
		&& !(btn_status.long_pushed_bmp[btn_col_idx] & (1<<i))){
			if(btn_ms_cnt[btn_col_idx*BTN_NUM_COL + i] > LONG_PRESS_TIME_MS){
				btn_status.long_pushed_bmp[btn_col_idx] |= (1<<i);
				p->on_long_push(btn_col_idx*BTN_NUM_COL + i);
			}
		}
	}
	digitalWrite(btn_select_pins[btn_col_idx], HIGH);	
	btn_col_idx = (btn_col_idx+1)%BTN_NUM_COL;
}

static void upd_shift_reg(led_matrix* lm){
}
static void switch_matrix_ui(led_matrix* next, led_matrix* prev){
	prev->set_hw(NULL);
	next->set_hw(&hw);

	for(int i=0; i<NR_LEDS; i++){
		struct led_status* tmp = next->get_led_status(i);
		if(tmp->bmp){
			uint8_t idx = BIT::get_highest_bit_set(tmp->bmp);
			hw.upd_pxl(i, tmp->color[idx], idx);
		}
		else {
			hw.upd_pxl(i, 0, 0);
		}
	}
	hw.refresh_matrix(0);
}
static void setup_matrix(){
	tft.begin();
	tft.fillScreen(ILI9341_BLACK);
	uint8_t x = tft.readcommand8(ILI9341_RDMODE);
	Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
	x = tft.readcommand8(ILI9341_RDMADCTL);
	Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
	x = tft.readcommand8(ILI9341_RDPIXFMT);
	Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
	x = tft.readcommand8(ILI9341_RDIMGFMT);
	Serial.print("Image Format: 0x"); Serial.println(x, HEX);
	x = tft.readcommand8(ILI9341_RDSELFDIAG);
	Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);

	hw.resetLcd();

	int i;
	grd_cnt = 0;

	btn_col_idx = 0;
	flag_btn_active = false;

	init_rd_cbck(&btn_matrix_digitalRead, 0);

	for(i=0;i<BTN_NUM_COL;i++){
		btn_row[i].init_ArrBounce(btn_read_pins, BOUNCE_TIME, BTN_NUM_ROW, (PinInterface *) &cpuPinReader);
		btn_status.pushed_bmp[i] = 0x0;
		btn_status.long_pushed_bmp[i] = 0x0;
	}

	for (i=0;i<BTN_NUM_COL;i++){
		pinMode(btn_select_pins[i], OUTPUT);
		digitalWrite(btn_select_pins[i], HIGH);
	}

	// button row input lines
	for (i=0;i<BTN_NUM_ROW;i++){
		pinMode(btn_read_pins[i], INPUT_PULLUP);
//		pullUp(btn_read_pins[i], HIGH);
	}
}


#else

static void scan(prog* p){
}
static void upd_shift_reg(led_matrix* lm){
}
static void switch_matrix_ui(led_matrix* next, led_matrix* prev){
}
static void setup_matrix(){
	Serial.println("empty matrix");
	delay(1000);
}

#endif
