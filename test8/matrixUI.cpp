#include "matrixUI.h"

uint8_t matrixUI::get_mui_id(){
	return _mui_id;
}
void matrixUI::set_mui_id(uint8_t id){
	_mui_id = id;
}
matrixUI* matrixUI::get_mui(){
	return this;
}
