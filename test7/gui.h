#ifndef __GUI_H__
#define __GUI_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif


#define GUI_NR_LINES			3
#define GUI_NR_CHAR_PER_LINE 	8		

class gui {
	private:
		static char* buffer[GUI_NR_CHAR_PER_LINE];
	
	public:
		static void init_gui(void (*fct)(char**));
		static uint8_t upd_buf(char* line, int idx);
		static void (*refresh)(char**);

};

#endif