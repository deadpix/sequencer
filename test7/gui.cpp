#include "gui.h"

void gui::init_gui(void (*fct)(char**)){
	refresh = fct;
}

uint8_t gui::upd_buf(char* line, int idx){
	uint8_t res = 1;
	if(idx < GUI_NR_LINES){
		buffer[idx] = line;
	}
	else { 
		res = 0;
	}
	return res;
}