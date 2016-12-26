#include "btn_action.h"

//void btn_action::setup_btn_action(void (*on_push)(void*, uint8_t),void (*on_release)(void*, uint8_t), void* ptr){
//	btn_on_push = on_push;
//	btn_on_release = on_release;
//	object_ptr = ptr;
//}

//void btn_action::setup_btn_action_test(void (prog::*push)(void*, uint8_t), void (prog::*release)(void *, uint8_t), void* ptr){
//	on_push = push;
//	on_release = release;
//	test_object_ptr = ptr;
//}

void btn_action::set_current_prog(prog *p){
	curr_prog = p;
}
	
prog* btn_action::get_current_prog(){
	return curr_prog;
}