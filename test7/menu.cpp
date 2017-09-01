#include "menu.h"

static led_matrix* default_clbk_func(void* ptr, uint8_t id1, uint8_t id2, led_matrix* lm){
	Serial.print("program ");
	Serial.print(id1);
	Serial.println(" has no menu function");
	return NULL;
}
menu::menu(){
	struct menu_clbk default_clbk;
	default_clbk.clbk_on_push = default_clbk_func;
	default_clbk.clbk_on_release = default_clbk_func;
	default_clbk.obj_ptr = NULL;
	
	for(int i=0;i<MATRIX_NR_ROW;i++){
		menu_clbk_arr[i] = default_clbk;
	}
}

menu::~menu(){	
}

led_matrix* menu::get_menu_led_matrix(){
	return &menu_interface;
}

void menu::set_menu_clbk(uint8_t prog_id, menu_clbk_type on_push, menu_clbk_type on_release, void* ptr){
	struct menu_clbk tmp;
	tmp.clbk_on_push = on_push;
	tmp.clbk_on_release = on_release;
	tmp.obj_ptr = ptr;
	
	if(prog_id < MATRIX_NR_ROW)
		menu_clbk_arr[prog_id] = tmp;
}

led_matrix* menu::get_next_interface(){
	return next_interface;
}
prog* menu::get_next_prog(){
	return next_prog;
}

void menu::set_next_interface(led_matrix* lm){
	next_interface = lm;
}

void menu::set_next_prog(prog* p){
	next_prog = p;
}

void menu::menu_enter(){
}
void menu::menu_leave(){
}
void menu::menu_update(){
}



void menu::menu_on_push(uint8_t btn_id){
	uint8_t prog_id = btn_id / MATRIX_NR_ROW;
	uint8_t opt_id = btn_id % MATRIX_NR_COL;
	struct menu_clbk tmp = menu_clbk_arr[prog_id];
	
	led_matrix* tmp_lm = (tmp.clbk_on_push)(tmp.obj_ptr, prog_id, opt_id, &menu_interface);

}
void menu::menu_on_release(uint8_t btn_id){
	uint8_t prog_id = btn_id / MATRIX_NR_ROW;
	uint8_t opt_id = btn_id % MATRIX_NR_COL;
	struct menu_clbk tmp = menu_clbk_arr[prog_id];
	
	led_matrix* tmp_lm = (tmp.clbk_on_release)(tmp.obj_ptr, prog_id, opt_id, &menu_interface);
	
	if(tmp_lm){
		next_interface = tmp_lm;
		next_prog = static_cast<prog *>(tmp.obj_ptr);
	}
}
void menu::on_push(void* this_ptr, uint8_t btn_id){
	menu* myself = static_cast<menu *>(this_ptr);
	myself->menu_on_push(btn_id);
}
void menu::on_release(void* this_ptr, uint8_t btn_id){
	menu* myself = static_cast<menu *>(this_ptr);
	myself->menu_on_release(btn_id);
}

led_matrix* menu::get_led_matrix(void){
	return &menu_interface;
}