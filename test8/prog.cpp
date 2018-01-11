#include "prog.h"

void prog::set_prog_id(uint8_t id){
	matrixUI::set_mui_id(id);
}
uint8_t prog::get_prog_id(){
	return matrixUI::get_mui_id();
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
void prog::set_gui(gui* gui_ptr){
	_gui = gui_ptr;
}
void prog::set_title(char* str){
	_title = str;
}
void prog::display_title(){
	_gui->upd_buf(_title, 0);
	_gui->refresh();
}