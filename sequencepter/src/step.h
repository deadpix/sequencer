#ifndef __STEP_H__
#define __STEP_H__

#include <stdint.h>

#include "types.h"
#include "clk.h"

class step;

#include "node.h"

struct note {
	uint8_t  velocity;
	uint16_t pitch;
};
struct serialized_tree_t {
	uint16_t * buf;
	uint8_t nr_byte;
};

class step {
	private:
		volatile bool flag_active;
		uint8_t gate_len_per; // percentage
		uint32_t gate_len_ms; // percentage
		bool _flg_gate;
		elapsedMillis gate_elapsed;
		
		step* _next;
		step* _prev;
	
		uint8_t _color;


	public:
		step();
		~step();
		struct note _note;
		struct clk_def _clk_def;
		node* _node;	
	
		bool reset_gate();
		bool upd_gate();
	
		uint8_t get_step_gate_len();
		void set_step_gate_len(uint32_t ms, uint8_t len);
		void upd_step_gate_len(uint32_t);

		bool is_step_active();
		void set_step_active();
		void clr_step_active();
		
		void set_next_step(step*);
		step* get_next_step();
		void set_prev_step(step*);
		step* get_prev_step();

		void set_step_color(uint8_t);
		uint8_t get_step_color();
		
		void step_set_note(uint8_t, uint16_t);

		static void serialize_tree(step* s, struct serialized_tree_t* s_tree);
		static void deserialize_tree(node * root, struct serialized_tree_t * s_tree);
};

#endif
