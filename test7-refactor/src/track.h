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

struct signature_change {
	uint8_t num;
	uint8_t denom;
	step* s;
	uint8_t color;
	led_toogle* signature_ui;
};

class track {
	private:
		uint16_t curr_step_id;
		uint8_t _max_step;
		uint8_t _track_id;
		uint8_t _out_id;
		bool	_play;	
		step*	_cur_step;
		step*	_first_step;
		step*  	_last_step;
		
		uint16_t _mst_clk_cnt;

//		uint8_t track_len;
		clk _c;
//		elapsedMillis elapsed_ms;
		led_matrix _lm;
		bool mute_flg;
//		LinkedList<track *> sub_track_list;
		node head;

	public:
//		step arr_step[NR_STEP];
		LinkedList<step *> _step_list;
//		LinkedList<step *> _sub_step_list;
		LinkedList<struct signature_change*> _signature_change_list;
		
		led_toogle _step_animation[DEFAULT_STEP_PER_SEQ];
//		led_toogle _step_animation;

//		step* _mtx_btn_to_step[NR_STEP];
		node* _mtx_to_node[NR_STEP];
		node* get_node_from_matrix(uint8_t);
		void  set_node_in_matrix(uint8_t, node*);
		static void chain_step_from_node_list(LinkedList<node *> *list, step* start, step* end);
		void create_tree(node*, uint8_t, uint8_t, uint8_t, uint8_t);
//		step* arr_step;
		struct clk_def _clk_def;

		static bool delete_step(LinkedList<step *> *l, step* s);

		int  add_signature_change(step* s, uint8_t num, uint8_t denom, uint8_t color);
		int  del_signature_change(step* s);
		void show_signature_change(uint32_t);

		void show_children_node(node*);
		void show_parent_nodes(node*, node*);

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
			return _first_step;	
		}
		step* get_last_step(){
			return _last_step;
		}
		void  set_last_step(step * s){
			_last_step = s;
		}
		void set_all_step_note(uint16_t);
//		void set_step_note(uint16_t, uint8_t );
		
//		bool is_step_on(uint8_t id);
		bool next_step(uint32_t);
		uint8_t get_current_step();
//		void toogle_step_x(uint8_t id);
		void step_reset();

		uint32_t check_event(uint32_t, uint16_t/*bool, clk *c*/);
		void init_hw_clbk(void (*fct)(uint16_t, uint8_t, uint8_t));

		void init_menu();
		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){ UNUSED(btn_id); };
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id){ UNUSED(btn_id); };

		void mute();
		void unmute();
		void toogle_mute();
		void toogle_play();	
		void set_play(bool);
		bool is_playing();
	
		void update_ui(uint32_t mst_ms, uint16_t mst_step);
};

#endif
