#include "btn_action.h"

void btn_action::set_current_prog(prog *p){
	curr_prog = p;
}
	
prog* btn_action::get_current_prog(){
	return curr_prog;
}