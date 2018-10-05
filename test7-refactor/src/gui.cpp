#include <stdint.h>

#include "types.h"
#include "gui.h"


static void (*refresh_fct)(char**);

static void dummy_fct(char** str){
	UNUSED(str);
//	Serial.println(str[0]);
//	Serial.println(str[1]);
//	Serial.println(str[2]);
}

//gui::gui() : buffer { "a", "b", "c" }{
gui::gui(){
	refresh_fct = dummy_fct;
}

gui::~gui(){
}

void gui::init_gui(void (*fct)(char**)){
	refresh_fct = fct;
}

uint8_t gui::upd_buf(char* str, int line_idx){
	uint8_t res = 1;
	
	if(line_idx < GUI_NR_LINES){
		buffer[line_idx] = str;
	}
	else { 
		res = 0;
	}
	return res;
}
void gui::refresh(){
	refresh_fct( (char**) &buffer);
}
