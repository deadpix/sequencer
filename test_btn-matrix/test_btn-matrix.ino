#define NR_COL  2	
#define NR_ROW 	2

#include "Bounce_array.h"

#define BOUNCE_TIME	5 


//static uint8_t col_pin[NR_COL] = {8,9};
//static uint8_t row_pin[NR_ROW] = {10,11};
static uint8_t col_pin[NR_COL] = {17,16};
static uint8_t row_pin[NR_ROW] = {15,14};




//static uint8_t col_pin[NR_COL] = {2,3,4};
//static uint8_t row_pin[NR_ROW] = {5};

static ArrBounce btn_row[NR_COL];


uint8_t selected_col;

IntervalTimer ui_timer;


uint8_t btn_matrix_digitalRead(uint8_t pin){
	return digitalRead(pin);
}

static void init(){
	selected_col = 0;
	for(int i=0;i<NR_ROW;i++){
		pinMode(row_pin[i], INPUT_PULLUP);
		
	}


	for(int i=0;i<NR_COL;i++){
		btn_row[i].init_ArrBounce(row_pin, BOUNCE_TIME, NR_ROW, &btn_matrix_digitalRead);
		pinMode(col_pin[i], OUTPUT);
		digitalWrite(col_pin[i], HIGH);
	}
	
}
static void scan(){
	uint8_t i;

	digitalWrite(col_pin[selected_col], LOW);

	for(i=0; i<NR_ROW;i++){
		if(btn_row[selected_col].update(i)){
		Serial.println(i);
			if(btn_row[selected_col].read(i) == HIGH){
				Serial.print("realease btn ");
				Serial.println(selected_col*NR_ROW + i);
			} else {
				Serial.print("push btn ");
				Serial.println(selected_col*NR_ROW + i);	
			}
		}
	
	}
	digitalWrite(col_pin[selected_col], HIGH);
	selected_col = (selected_col + 1) % NR_COL;
}
void setup(){
	init();
//	ui_timer.begin(scan, 1000);	
}

void loop(){
//	Serial.println("Loop");
//	delay(500);
	scan();
}
