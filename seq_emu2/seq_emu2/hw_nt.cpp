#include "../../sequencepter/src/bit.h"
#include "hw_nt.h"
#include <hw_debug.h>

//hw_nt::hw_nt(){
//	_lm = NULL;
//}

hw_nt::hw_nt(QPushButton** matrix_btn){
	_matrix_btn = matrix_btn; 
	_lm = NULL;
}

//void hw_nt::set_matrix_btn(QPushButton* matrix_btn[64]){
//	_matrix_btn = matrix_btn;
//}

void hw_nt::refresh_matrix(uint16_t id){
	if(_lm){
		struct led_status* tmp = _lm->get_led_status(id);
		if(tmp->bmp){
			uint8_t idx = BIT::get_highest_bit_set(tmp->bmp);
			_matrix_btn[id]->setStyleSheet(color_arr[tmp->color[idx]]);
			dbg::printf("refresh %d color %s\n",id,color_arr[tmp->color[idx]]);
		}
		else {
			dbg::printf("refresh %d no color\n",id);
			_matrix_btn[id]->setStyleSheet(color_arr[0]);
		}
		_matrix_btn[id]->update();
	
	}
	else {
		dbg::printf("led matrix buffer is NULL...\n");
	}

}

void hw_nt::upd_pxl(uint16_t id, uint8_t color, uint8_t brightness){
	if(_lm){
		_matrix_btn[id]->setStyleSheet(color_arr[color]);
		_matrix_btn[id]->update();
	}
}


void hw_nt::set_lm(led_matrix* lm){
	_lm = lm;
}

void hw_nt::switch_matrix(led_matrix* next, led_matrix* prev){
	_lm = next;
	prev->set_hw(NULL);
	next->set_hw(this);
	for(int i=0; i<NR_LEDS; i++){
		struct led_status* tmp = _lm->get_led_status(i);
		if(tmp->bmp){
			uint8_t idx = BIT::get_highest_bit_set(tmp->bmp);
			_matrix_btn[i]->setStyleSheet(color_arr[tmp->color[idx]]);
		}
		else {
			_matrix_btn[i]->setStyleSheet(color_arr[0]);
		}
		 _matrix_btn[i]->update();
	}
}
