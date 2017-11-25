#include "param.h"

//param::param(){
//}

//param::~param(){
//}

led_matrix* param::get_led_matrix(){
	return &_lm;
}
prog* param::get_prog(){
	return _p;
}
void  param::set_prog(prog* p){
	_p = p;
}

//void param::on_push(uint8_t btn_id){
//}
//void param::on_release(uint8_t btn_id){
//}
//void param::update_ui(){
//}