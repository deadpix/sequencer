#ifndef __TRACK_H__
#define __TRACK_H__

#include <stdint.h>
//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif

#include "types.h"
#include "led_matrix.h"
#include "menu.h"
#include "prog.h"
#include "led_toogle.h"
#include "step.h"
#include "clk.h"
#include "node.h"

#define SEQ_NR_LOOP_SETTING	6

struct signature_change {
	uint8_t num;
	uint8_t denom;
	step* s;
	uint8_t color;
	led_toogle* signature_ui;
};

struct loop_step {
	step* first;
	step* last;
};

class track {
	private:
//		uint16_t curr_step_id;
		uint8_t _max_step;
		uint8_t _track_id;
		uint8_t _out_id;
		bool	_play;	

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
		bool mute_flg;
//		LinkedList<track *> sub_track_list;
		node head;
		void _init_animate_parents(step* cur);
		void _upd_animate_parents(step* cur);
		uint8_t track_color_;


	public:
		void set_track_color(uint8_t color){track_color_ = color;};
		uint8_t get_track_color(){ return track_color_;};
		LinkedList<struct signature_change*> _signature_change_list;
		
		led_toogle _step_animation[DEFAULT_STEP_PER_SEQ];
//		led_toogle _step_animation;

//		step* _mtx_btn_to_step[NR_STEP];
		node* _mtx_to_node[NR_STEP];
		node* get_node_from_matrix(uint8_t);
		void  set_node_in_matrix(uint8_t, node*);
		static void chain_step_from_node_list(LinkedList<node *> *list, step* start, step* end);
		static void set_loop(step* first, step* last);
		void create_tree(node*, uint8_t, uint8_t, uint8_t, uint8_t);
//		step* arr_step;
		volatile struct clk_def _clk_def;
		void set_clk_def_lock(uint8_t num, uint8_t denom); 		

		static bool delete_step(LinkedList<step *> *l, step* s);

		void mask_npo_nodes(uint8_t npo);
		void show_children_node(node*);
		void show_parent_nodes(node*, node*);
		void show_current_step_nodes_no_irq();

		track();
//		track(uint8_t);
		~track();
		
		led_matrix * get_led_matrix();
		clk* get_clk();

		void set_max_step(uint8_t max);
		uint8_t get_max_step();
		
		void set_track_id(uint8_t id);
		uint8_t get_track_id();

		void set_out_id(uint8_t id);
		uint8_t get_out_id();

		step* get_first_step(){
			return loop_step_[cur_loop_].first;
		}
		step* get_last_step(){
			return loop_step_[cur_loop_].last;
		}
		void  set_first_step(step * s){
			loop_step_[cur_loop_].first = s;
		}

		void  set_last_step(step * s){
			loop_step_[cur_loop_].last = s;
		}
		void set_all_step_note(uint16_t);
//		void set_step_note(uint16_t, uint8_t );
		
//		bool is_step_on(uint8_t id);
		bool next_step(uint32_t);
//		uint8_t get_current_step();
//		void toogle_step_x(uint8_t id);
		void step_reset();

		uint8_t check_event(uint32_t, uint16_t/*bool, clk *c*/);
		void init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t));

//		void init_menu();
//		void on_push(uint8_t btn_id);
//		void on_long_push(uint8_t btn_id){ UNUSED(btn_id); };
//		void on_release(uint8_t btn_id);
//		void on_long_release(uint8_t btn_id){ UNUSED(btn_id); };

		void mute();
		void unmute();
		void toogle_mute();
		void toogle_play();	
		void set_play(bool);
		bool is_playing();

		void set_current_loop(uint8_t loop_id);
		uint8_t get_current_loop(){ return cur_loop_; };

		void upd_animate_parents_no_irq();
		void init_animate_parents_no_irq();
//		void update_ui(uint32_t mst_ms, uint16_t mst_step);
};

#endif
