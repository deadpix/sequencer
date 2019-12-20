#ifndef __TRACK_H__
#define __TRACK_H__

#include <stdint.h>

#include "types.h"
#include "led_matrix.h"
#include "menu.h"
#include "prog.h"
#include "led_toogle.h"
#include "step.h"
#include "clk.h"
#include "node.h"

#define SEQ_NR_LOOP_SETTING	6

#define TRACK_PLAY_STATE    (1<<0)
#define TRACK_PAUSE_STATE   (1<<1)
#define TRACK_STOP_STATE    (1<<2)



/*
struct signature_change {
	uint8_t num;
	uint8_t denom;
	step* s;
	uint8_t color;
	led_toogle* signature_ui;
};
*/

struct loop_step {
	step* first;
	step* last;
};

class track {
	private:
//		uint16_t curr_step_id;
		uint8_t _max_step;
//		uint8_t _track_id;

		//TODO _out_id should be accessed via midi controller
		uint8_t _out_id;
        uint8_t _state;

		step*	_cur_step;
		step*	_prev_step;
//		step*	_first_step;
//		step* 	_last_step;

		struct loop_step loop_step_[SEQ_NR_LOOP_SETTING]; 
		uint8_t cur_loop_;

		uint16_t _mst_clk_cnt;

//		uint8_t track_len;
		clk _c;

//		elapsedMillis elapsed_ms;
		led_matrix _lm;
//		bool mute_flg;
//		LinkedList<track *> sub_track_list;
        node * head;
		uint8_t track_color_;
		led_toogle _step_animation[DEFAULT_STEP_PER_SEQ];

		bool next_step(uint32_t);
		void _init_animate_parents(step* cur);
		void _upd_animate_parents(step* cur);

	public:
        void set_track_color(uint8_t color){ track_color_ = color; }
        uint8_t get_track_color(){ return track_color_; }
//		LinkedList<struct signature_change*> _signature_change_list;

		node* _mtx_to_node[NR_STEP];
		node* get_node_from_matrix(uint8_t);
        node* get_root_node(){ return head; }
        void  set_root_node(node * n){ head = n; }

		void  set_node_in_matrix(uint8_t, node*);
		static void chain_step_from_node_list(LinkedList<node *> *list, step* start, step* end);
//		static void set_loop(step* first, step* last);
		void set_first_in_loop(step* new_first, uint8_t loop_id);
		void set_last_in_loop(step* new_last, uint8_t loop_id);

		void create_tree(node*, uint8_t, uint8_t, uint8_t, uint8_t);
		volatile struct clk_def _clk_def;
		void set_clk_def_lock(uint8_t num, uint8_t denom); 		

		static bool delete_step(LinkedList<step *> *l, step* s);

		void mask_npo_nodes(uint8_t npo);
		void show_children_node(node*);
		void show_parent_nodes(node*, node*);
		void show_current_step_nodes_no_irq();

		track();
		~track();
		
		led_matrix * get_led_matrix();
		clk* get_clk();

		void set_max_step(uint8_t max);
		uint8_t get_max_step();

		void set_out_id(uint8_t id);
		uint8_t get_out_id();

        step* get_first_step(){ return loop_step_[cur_loop_].first; }
        step* get_last_step(){	return loop_step_[cur_loop_].last; }
        step* get_first_step(uint8_t loop_id){ return loop_step_[loop_id].first; }
        step* get_last_step(uint8_t loop_id){ return loop_step_[loop_id].last; }
        void  set_first_step(step * s){	loop_step_[cur_loop_].first = s; }
        uint8_t get_current_loop_id(){	return cur_loop_;}
        void  set_last_step(step * s){ loop_step_[cur_loop_].last = s; }

		void set_all_step_note(uint16_t);
		
		void step_reset();

		uint8_t check_event(uint32_t, uint16_t/*bool, clk *c*/);
		void init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t));

//		void mute();
//		void unmute();
//		void toogle_mute();
//		void toogle_play();	
//        void set_play(bool);
        void set_state(uint8_t state) { _state = state; }
        uint8_t get_state() { return _state; }
        uint8_t track_is_playing() { return _state & TRACK_PLAY_STATE; }
        uint8_t track_is_stopped() { return _state & TRACK_STOP_STATE; }
        uint8_t track_is_paused() { return _state & TRACK_PAUSE_STATE; }
        void play_track(){ _state = TRACK_PLAY_STATE; }
        void stop_track(){ _state = TRACK_STOP_STATE; }
        void pause_track(){ _state = TRACK_PAUSE_STATE; }

//        bool is_playing();

		void set_current_loop(uint8_t loop_id);
        uint8_t get_current_loop(){ return cur_loop_; }

		void upd_animate_parents_no_irq();
		void init_animate_parents_no_irq();
};

#endif
