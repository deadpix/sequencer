#include "led_matrix.h"

led_matrix::led_matrix(){
	for(int i=0;i<NUM_LED_COLUMNS;i++){
		for(int j=0;j<NUM_LED_ROWS;j++){
			status[i][j] = LED_OFF;
		}
	}
}

led_matrix::~led_matrix(){
}
	
boolean led_matrix::set_pixel(uint8_t pixel_id){
	uint8_t i = pixel_id / NUM_LED_ROWS;
	uint8_t j = pixel_id % NUM_LED_COLUMNS;
	boolean ret = status[i][j] ^ true; 
	status[i][j] = LED_ON;
	return ret;
}
boolean led_matrix::clr_pixel(uint8_t pixel_id){
	uint8_t i = pixel_id / NUM_LED_ROWS;
	uint8_t j = pixel_id % NUM_LED_COLUMNS;
	boolean ret = status[i][j] ^ false; 
	status[i][j] = LED_OFF;
	return ret;
}
boolean led_matrix::get_pixel_status(uint8_t pixel_id){
	return status[(pixel_id/NUM_LED_ROWS)][(pixel_id%NUM_LED_COLUMNS)];
}
void led_matrix::set_pixel_status(uint8_t pixel_id, boolean s){
	status[(pixel_id/NUM_LED_ROWS)][(pixel_id%NUM_LED_COLUMNS)] = s;
}

void led_matrix::toogle_pixel_status(uint8_t row_id, uint8_t col_id){
	Serial.println("tooglle pixel");
	status[row_id][col_id] = !status[row_id][col_id];
}
