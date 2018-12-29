#ifndef HW_NT_H
#define HW_NT_H

#include "../../sequencepter/src/hw.h"
#include "../../sequencepter/src/led_matrix.h"

#include "configuration.h"
#include <QPushButton>
//#include "mainwindow.h"

class hw_nt : public hw {
	private:	
		QPushButton** _matrix_btn;
		led_matrix* _lm;	

	public:
//		hw_nt();
		hw_nt(QPushButton** matrix_btn);
		~hw_nt(){};
		void refresh_matrix(uint16_t id);
		void upd_pxl(uint16_t id, uint8_t color, uint8_t brightness);
		void set_lm(led_matrix* lm);
//		void set_matrix_btn(QPushButton* matrix_btn[64]);
		void switch_matrix(led_matrix* next, led_matrix* prev);
};

#endif
