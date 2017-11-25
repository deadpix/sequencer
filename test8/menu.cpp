#include "menu.h"

static led_matrix* default_clbk_func(/*void* ptr, */uint8_t id1, uint8_t id2/*, led_matrix* lm*/){
	Serial.print("program ");
	Serial.print(id1);
	Serial.println(" has no menu function");
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
		_mui_arr = NULL;
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
void menu::set_menu_mui_entry(uint8_t prog_id, matrixUI* mui){
	_mui_arr[prog_id] = mui;
}

prog* menu::get_next_prog(){
	return next_prog;
}
void menu::set_next_prog(prog* p){
	next_prog = p;
}

matrixUI* menu::get_next_mui(){
	return _next_mui;
}
void menu::set_next_mui(matrixUI* mui){
	_next_mui = mui;
}


void menu::menu_enter(){
	for(int i=0;i<MATRIX_NR_COL;i++){
		if(_mui_arr[i])
			_mui_arr[i]->menu_enter();
//		prog* p = prog_arr[i];
//		if(p)
//			p->menu_enter();
	}
}
void menu::menu_leave(){
	for(int i=0;i<MATRIX_NR_COL;i++){
		if(_mui_arr[i])
			_mui_arr[i]->menu_leave();

//		prog* p = prog_arr[i];
//		if(p)
//			p->menu_leave();
	}
}
void menu::menu_update(){
	for(int i=0;i<MATRIX_NR_COL;i++){
		if(_mui_arr[i])
			_mui_arr[i]->menu_update();

//		prog* p = prog_arr[i];
//		if(p)
//			p->menu_update();			
	}
}

// DUMMY FUNCTIONS
int menu::menu_on_push(uint8_t, uint8_t){
}
int menu::menu_on_release(uint8_t, uint8_t){
}

void menu::on_push(uint8_t btn_id){
	uint8_t prog_id = btn_id / MATRIX_NR_ROW;
	uint8_t opt_id = btn_id % MATRIX_NR_COL;

	if(_mui_arr[prog_id])
		_mui_arr[prog_id]->menu_on_push(prog_id, opt_id);
//	prog* p = prog_arr[prog_id];
//	if(p)
//		p->menu_on_push(prog_id, opt_id);
}
void menu::on_release(uint8_t btn_id){
	uint8_t prog_id = btn_id / MATRIX_NR_ROW;
	uint8_t opt_id = btn_id % MATRIX_NR_COL;
	int ret;
	
//	prog* p = prog_arr[prog_id];
//	if(p)
//		ret = p->menu_on_release(prog_id, opt_id);
	
	if(_mui_arr[prog_id])
		ret = _mui_arr[prog_id]->menu_on_push(prog_id, opt_id);

	if(ret){
//		next_prog = p;
		_next_mui = _mui_arr[prog_id];
	}
}

led_matrix* menu::get_led_matrix(void){
	return &menu_interface;
}
void menu::update_ui(){
}
