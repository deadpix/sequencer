#include "param.h"

led_matrix* param::get_led_matrix(){
	return &_lm;
}
prog* param::get_prog(){
	return _p;
}
void  param::set_prog(prog* p){
	_p = p;
}