#include "menu.h"

static led_matrix* default_clbk_func(/*void* ptr, */uint8_t id1, uint8_t id2/*, led_matrix* lm*/){
	UNUSED(id1);
	UNUSED(id2);
//	Serial.print("program ");
//	Serial.print(id1);
//	Serial.println(" has no menu function");
	return NULL;
}
static void default_entry_func(void){
}
menu::menu(){
	struct menu_clbk default_clbk;
	default_clbk.clbk_on_push = default_clbk_func;
	default_clbk.clbk_on_release = default_clbk_func;
	default_clbk.obj_ptr = NULL;
	default_clbk.enter = default_entry_func;
	default_clbk.leave = default_entry_func;
	default_clbk.update = default_entry_func;
	
	for(int i=0;i<MATRIX_NR_COL;i++){
		menu_clbk_arr[i] = default_clbk;
		prog_arr[i] = NULL;
	}	
}

menu::~menu(){	
}

led_matrix* menu::get_menu_led_matrix(){
	return &menu_interface;
}

void menu::set_menu_prog_entry(uint8_t prog_id, prog* p){
	prog_arr[prog_id] = p;
}

prog* menu::get_next_prog(){
	return next_prog;
}

void menu::set_next_prog(prog* p){
	next_prog = p;
}

void menu::menu_enter(){
	for(int i=0;i<MATRIX_NR_COL;i++){
		prog* p = prog_arr[i];
		if(p)
			p->menu_enter();
	}
}
void menu::menu_leave(){
	for(int i=0;i<MATRIX_NR_COL;i++){
		prog* p = prog_arr[i];
		if(p)
			p->menu_leave();
	}
}
void menu::menu_update(){
	for(int i=0;i<MATRIX_NR_COL;i++){
		prog* p = prog_arr[i];
		if(p)
			p->menu_update();			
	}
}

// DUMMY FUNCTIONS
int menu::menu_on_push(uint8_t, uint8_t){
	return 0;
}
int menu::menu_on_release(uint8_t, uint8_t){
	return 0;
}

void menu::on_push(uint8_t btn_id){
	uint8_t prog_id = btn_id / MATRIX_NR_ROW;
	uint8_t opt_id = btn_id % MATRIX_NR_COL;

	prog* p = prog_arr[prog_id];
	if(p)
		p->menu_on_push(prog_id, opt_id);
}
void menu::on_release(uint8_t btn_id){
	uint8_t prog_id = btn_id / MATRIX_NR_ROW;
	uint8_t opt_id = btn_id % MATRIX_NR_COL;
	int ret = 0;
	
	prog* p = prog_arr[prog_id];

	if(p)
		ret = p->menu_on_release(prog_id, opt_id);
	
	if(ret){
		next_prog = p;
	}
}

led_matrix* menu::get_led_matrix(void){
	return &menu_interface;
}
void menu::update_ui(uint32_t mst_ms, uint16_t mst_step){
	UNUSED(mst_ms);
	UNUSED(mst_step);
}
