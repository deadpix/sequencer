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

void sequencer::start_all_tracks(){
    for(int i=0;i<SEQUENCER_NR_TRACK;i++){
        track_arr[i].play_track();
    }
}
void sequencer::pause_all_tracks(){
    for(int i=0;i<SEQUENCER_NR_TRACK;i++){
        track_arr[i].pause_track();
    }
}
void sequencer::stop_all_tracks(){
    for(int i=0;i<SEQUENCER_NR_TRACK;i++){
        track_arr[i].stop_track();
    }
}

void sequencer::reset_all(){
	for(int i=0;i<SEQUENCER_NR_TRACK;i++){
		track_arr[i].step_reset();
	}
}

static void dump_serialized_track(const struct serialized_tree_t * st){
    dbg::printf("Dump tracks %d bytes:\n",st->header.tree_byte_sz);
    dbg::printf("header: magic %u number of nodes %d number of steps %d :\n"
                ,st->header.magic,st->header.nr_nodes,st->header.nr_steps);

    for(int i=0; i<st->header.tree_byte_sz; i++){
        dbg::printf("%02x-",st->buf[i]);
	}
    int idx = st->header.tree_byte_sz;
    dbg::printf("\nDump node data (size %d) idx=%d\n",sizeof(struct serialization_data_t),idx);
    for(int i=0; i<st->header.nr_nodes; i++){
        dbg::printf("node %d\n",i);
        dbg::printf("\t- gate length: %u\n",st->buf[idx+0]);
        dbg::printf("\t- velocity   : %u\n",st->buf[idx+1]);
        dbg::printf("\t- pitch      : %u%u\n",st->buf[idx+3],st->buf[idx+2]);
        dbg::printf("\t- numerator  : %u\n",st->buf[idx+4]);
        dbg::printf("\t- denominator: %u\n",st->buf[idx+5]);
        dbg::printf("\t- color      : %u\n",st->buf[idx+6]);
        dbg::printf("\t- matrix id  : %u\n",st->buf[idx+7]);
        dbg::printf("\n");
        idx += sizeof(struct serialization_data_t);
    }
    dbg::printf("\n");
}

void sequencer::serialize_current_track(struct serialized_tree_t * st, uint16_t* serialized_data_sz){
	step * s = get_current_track()->get_first_step();
    node * n = get_current_track()->get_root_node();

    step::serialize_tree(s, n, st, serialized_data_sz);
	dump_serialized_track(st);

}
int sequencer::deserialize_current_track(struct serialized_tree_t * st){
    node * old_head = get_current_track()->get_root_node();
    node * new_head;
    int ret = step::deserialize_tree(new_head, st);
    if(ret == SERIALIZATION_OK){
        get_current_track()->set_root_node(new_head);
        old_head->delete_tree();
    }
    return ret;
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
    track* t;

    for(uint8_t i=0;i<SEQUENCER_NR_TRACK;i++){
        t = _s->get_track(i);
        dbg::printf("track is stopped? %d\n",t->track_is_stopped());
        if(!t->track_is_stopped()){
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
}
