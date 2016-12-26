#include "menu.h"

static led_matrix* default_clbk_func(void* ptr, uint8_t id1, uint8_t id2, led_matrix* lm){
	Serial.print("program ");
	Serial.print(id1);
	Serial.println(" has no menu callback function");
	return NULL;
}

menu::menu(){
	struct menu_clbk default_clbk;
	default_clbk.clbk = default_clbk_func;
	default_clbk.obj_ptr = NULL;
	
	for(int i=0;i<NUM_LED_ROWS;i++){
		menu_clbk_arr[i] = default_clbk;
	}
}

menu::~menu(){	
}

led_matrix* menu::get_menu_led_matrix(){
	return &menu_interface;
}

void menu::set_menu_clbk(uint8_t prog_id, menu_clbk_type clbk, void* ptr){
	struct menu_clbk tmp;
	tmp.clbk = clbk;
	tmp.obj_ptr = ptr;
	
	if(prog_id < NUM_LED_ROWS)
		menu_clbk_arr[prog_id] = tmp;
}

led_matrix* menu::get_next_interface(){
	return next_interface;
}
void menu::set_next_interface(led_matrix* lm){
	next_interface = lm;
}

void menu::menu_on_push(uint8_t btn_id){
	;
}
void menu::menu_on_release(uint8_t btn_id){
	uint8_t prog_id = btn_id / NUM_LED_ROWS;
	uint8_t opt_id = btn_id % NUM_LED_COLUMNS;
	struct menu_clbk tmp = menu_clbk_arr[prog_id];
	
	led_matrix* tmp_lm = (tmp.clbk)(tmp.obj_ptr, prog_id, opt_id, &menu_interface);
	
	if(tmp_lm)
		next_interface = tmp_lm;
//	if(flag)
//		next_interface = prog_interface[prog_id];
//	else 
//		next_interface = NULL;
}

//void menu::init_prog_interface_pointer(uint8_t prog_id, led_matrix * lm){
//	if(prog_id < NUM_LED_ROWS)
//		prog_interface[prog_id] = lm;
//}
//void menu::callback_on_push(void * this_ptr, uint8_t btn_id){
//	menu* myself = static_cast<menu *>(this_ptr);
//	myself->menu_on_push(btn_id);
//}
//void menu::callback_on_release(void * this_ptr, uint8_t btn_id){
//	menu* myself = static_cast<menu *>(this_ptr);
//	myself->menu_on_release(btn_id);
//}
void menu::on_push(void* this_ptr, uint8_t btn_id){
	menu* myself = static_cast<menu *>(this_ptr);
	myself->menu_on_push(btn_id);
}
void menu::on_release(void* this_ptr, uint8_t btn_id){
	menu* myself = static_cast<menu *>(this_ptr);
	myself->menu_on_release(btn_id);
}