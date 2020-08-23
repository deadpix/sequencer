#define BTN_NUM_COL  	8	
#define BTN_NUM_ROW 	8

#include "Bounce_array.h"
#include <Adafruit_MCP23017.h>
#include <i2c_t3.h>

#define BOUNCE_TIME	5 
#define NR_CBCK		3

typedef uint8_t (*cbck_fct_rd)(uint8_t pin);
cbck_fct_rd cbck_fct_rd_arr[NR_CBCK];
Adafruit_MCP23017 mcp;
uint8_t btn_col_idx;


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


static uint8_t btn_select_pins[BTN_NUM_COL] = {0,1,2,3,4,5,6,7}; // ground switch
static uint8_t btn_read_pins[BTN_NUM_ROW] = {8,9,10,11,12,13,14,15};

static ArrBounce btn_row[BTN_NUM_COL];

uint8_t selected_col;

uint8_t btn_matrix_digitalRead(uint8_t pin){
	return mcp.digitalRead(pin);
}

static void init(){
	int i = 0;
	selected_col = 0;

	Serial.println("init");
	init_rd_cbck(btn_matrix_digitalRead, 0);

	mcp.begin(0x0);
	Wire.setClock(1000000);

	for(i=0;i<BTN_NUM_COL;i++){
		btn_row[i].init_ArrBounce(btn_read_pins, BOUNCE_TIME, BTN_NUM_ROW, &mcp_digitalRead, 0);
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
static void scan(){
	uint8_t i;

// 	Select current columns
	mcp.digitalWrite(btn_select_pins[btn_col_idx], LOW);

// 	Read the button inputs

	for (i=0; i<BTN_NUM_ROW; i++){
		if(btn_row[btn_col_idx].update(i)){
			if(btn_row[btn_col_idx].read(i) == HIGH){
				Serial.print("realease btn ");
				Serial.println(selected_col*BTN_NUM_ROW + i);
			} else {
				Serial.print("push btn ");
				Serial.println(selected_col*BTN_NUM_ROW + i);
			}
		}
	}
	mcp.digitalWrite(btn_select_pins[btn_col_idx], HIGH);	
	btn_col_idx = (btn_col_idx+1)%BTN_NUM_COL;
}

void setup(){
	init();
}

int loop_cnt;
void loop(){
	if(loop_cnt >= 10000){
		Serial.println("loop");
		loop_cnt = 0;
	}
	else {
		loop_cnt++;
	}
	scan();
}
