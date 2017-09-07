#include "prog.h"

void prog::set_prog_id(uint8_t id){
	prog_id = id;
}
uint8_t prog::get_prog_id(){
	return prog_id;
}
prog* prog::get_prog(){
	return this;
}

led_matrix* prog::get_menu_lm(){
	return _menu_lm;
}
void prog::set_menu_lm(led_matrix* lm){
	_menu_lm = lm;
}