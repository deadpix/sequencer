#ifndef __LED_MATRIX_H__
#define __LED_MATRIX_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif


#include "configuration.h"

#define LED_ON	true
#define LED_OFF	false

class led_matrix {
	private:
		boolean status[NUM_LED_ROWS][NUM_LED_COLUMNS]; 
	
	public:
		led_matrix();
		~led_matrix();
		boolean set_pixel(uint8_t pixel_id);
		boolean clr_pixel(uint8_t pixel_id);
		boolean get_pixel_status(uint8_t pixel_id);
		void set_pixel_status(uint8_t pixel_id, boolean s);
		void toogle_pixel_status(uint8_t row_id, uint8_t col_id);
};

#endif