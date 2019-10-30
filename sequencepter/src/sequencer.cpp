#include "sequencer.h"
#include <hw_debug.h>
sequencer::sequencer(){
	current_param_id = 0;
	for(int i=0;i<(MATRIX_NR_COL*MATRIX_NR_ROW);i++){
		fct_arr[i] = NULL;
	}
	_ls_ui.step_id = -1;
//	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
//		track_arr[i].set_track_id(i);
//	}
}

sequencer::~sequencer(){
}

track* sequencer::get_track(uint8_t track_id){
	return &track_arr[track_id];
}

track* sequencer::get_current_track(){
	return current;
}

void sequencer::set_current_track(uint8_t track_id){
	current = &track_arr[track_id];
}

void sequencer::set_current_param(uint8_t id){
	current_param_id = id;
}
uint8_t sequencer::get_current_param(){
	return current_param_id;
}

void sequencer::add_fct(fct_clbk* fct, uint8_t idx){
	fct_arr[idx] = fct;
}
fct_clbk* sequencer::get_fct(uint8_t idx){
	return fct_arr[idx];
}

uint8_t sequencer::check_events(uint32_t mst_ms, uint16_t mst_step, event** e){
	uint8_t res = 0;
	
	track* t;
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		t = &track_arr[i];
		res |= (t->check_event(mst_ms, mst_step)) << i;	
	}

	*e = new next_step_evt(this, res);
	(*e)->set_event_id(EVT_NEXT_STEP);

	return res;
}


void sequencer::set_track_start(bool play){
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		track_arr[i].set_play(play);
	}
}
void sequencer::reset_all(){
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		track_arr[i].step_reset();
	}
}

static void dump_serialized_track(const struct serialized_tree_t * st){
    dbg::printf("Dump tracks:\n");
	for(int i=0; i<st->nr_byte; i++){
        dbg::printf("%u ",st->buf[i]);
	}
    dbg::printf("\n");
}

void sequencer::serialize_current_track(struct serialized_tree_t * st){
	step * s = get_current_track()->get_first_step();

	step::serialize_tree(s, st);
	dump_serialized_track(st);

}
void sequencer::deserialize_current_track(struct serialized_tree_t * st){
	node * n = get_current_track()->get_root_node();

	step::deserialize_tree(n, st);
}



void sequencer::menu_enter(){
}
void sequencer::menu_leave(){
}
void sequencer::menu_update(uint32_t mst_ms, uint16_t mst_step){
}


int sequencer::menu_on_push(uint8_t func_id, uint8_t opt_id){	
	int ret = 1;
	UNUSED(func_id);
	UNUSED(opt_id);
	prog::display_title();
	return ret;
}
int sequencer::menu_on_release(uint8_t func_id, uint8_t opt_id){
	int ret = 1;
	for(int i=0; i<MATRIX_NR_COL; i++){
		if(i == opt_id){
			get_menu_lm()->save_n_set(LED_B_IDX, func_id * MATRIX_NR_ROW + i, BACKGROUND);
			current = get_track(i);
		}
		else {
			get_menu_lm()->clr_n_restore(func_id * MATRIX_NR_ROW + i, BACKGROUND);
		}
	}	
	return ret;
}

void sequencer::on_push(uint8_t btn_id){
//	current->on_push(btn_id);
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->on_push(btn_id);
	}
}
void sequencer::on_long_push(uint8_t btn_id){
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->on_long_push(btn_id);
	}
}
void sequencer::on_release(uint8_t btn_id){
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->on_release(btn_id);
	}
}
void sequencer::on_long_release(uint8_t btn_id){
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->on_long_release(btn_id);
	}
}

void sequencer::update_ui(uint32_t mst_ms, uint16_t mst_step){
	if(fct_arr[current_param_id]){
		fct_arr[current_param_id]->update_ui(mst_ms,mst_step);
	}
}
led_matrix* sequencer::get_led_matrix(){
	return current->get_led_matrix();
}

void next_step_evt::do_evt(){
	uint8_t res = 0;
	
//	if(_s->get_led_matrix()->get_hw())
//		Serial.println("ok");
//	else 
//		Serial.println("ko");

	track* t;
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){	
		t = _s->get_track(i);
		if(t == _s->get_current_track()){
		
			if(step_evt_bmp & (1<<i)){
				t->show_current_step_nodes_no_irq();
				t->init_animate_parents_no_irq();
			}
			else {
				t->upd_animate_parents_no_irq();
			}
		}
	}
}
