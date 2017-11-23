#ifndef __MENUUI_H__
#define __MENUUI_H__

#include "types.h"
#include "matrixUI.h"

// should be prog...
class menuUI : matrixUI {
	protected:
		led_matrix* _menu_lm;
	
	public:
		led_matrix* get_menu_lm();
		void set_menu_lm(led_matrix* lm);
		
		virtual void menu_enter() = 0;
		virtual void menu_leave() = 0;
		virtual void menu_update() = 0;
		virtual int  menu_on_push(uint8_t, uint8_t) = 0;
		virtual int  menu_on_release(uint8_t, uint8_t) = 0;
}


#endif
