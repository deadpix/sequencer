#include "gui.h"
#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

//////////////////////////
// MicroOLED Definition //
//////////////////////////
#define PIN_RESET 8  // Connect RST to pin 9
#define PIN_DC    9  // Connect DC to pin 8
#define PIN_CS    10 // Connect CS to pin 10
#define DC_JUMPER 0

#define OLED_X_OFFSET	8
#define OLED_Y_OFFSET	16

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // SPI declaration
//MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration
gui oled_gui;

static void print_oled(char* line, uint8_t off){
	oled.setCursor(0, off);
	oled.print(line);
}

static void refresh_oled(char** line_arr){
	oled.clear(PAGE);
	print_oled(line_arr[0], 0);
	print_oled(line_arr[1], OLED_Y_OFFSET);
	print_oled(line_arr[2], 2*OLED_Y_OFFSET);
	oled.display();
	
/*	
	oled.setCursor(0, 0);
	line_ptr = g_buf->buf[0].l;
	oled.print(*line_ptr);

	oled.setCursor(0, OLED_Y_OFFSET);
	line_ptr = g_buf->buf[1].l;
	oled.print(*line_ptr);

	oled.setCursor(0, 2*OLED_Y_OFFSET);
	line_ptr = g_buf->buf[2].l;
	oled.print(*line_ptr);	

	oled.display();
*/
}

gui* setup_oled(){
	oled_gui.init_gui(refresh_oled);
	oled.begin();    // Initialize the OLED
	oled.clear(ALL); // Clear the display's internal memory
	oled.display();  // Display what's in the buffer (splashscreen)
	delay(1000);     // Delay 1000 ms
	oled.clear(PAGE); // Clear the buffer.
	oled.setFontType(1);
	oled.setCursor(0, 0);
	oled.print("OLED init done");
	oled.display();
	return &oled_gui;
}
