#include "step.h"
#include "led_matrix.h"
#include <hw_debug.h>
#include "bit.h"

step::step(){
	flag_active = false;
	gate_len_per = 20;
	_flg_gate = false;
	_note.velocity = 127;
	_note.pitch = 37;
	_color = LED_R_IDX;
	_clk_def.numerator = 1;
	_clk_def.denominator = 1;
}

step::~step(){
	dbg::printf("delete step\n");
}

void step::step_set_note(uint8_t vel, uint16_t pitch){
	_note.velocity = vel;
	_note.pitch = pitch;
}

bool step::reset_gate(){
	gate_elapsed = 0;
	_flg_gate = true;
	return _flg_gate;
}
bool step::upd_gate(){
	bool res = false;

	if( (gate_elapsed > gate_len_ms) && _flg_gate){
		res = true;
		_flg_gate = false;
	}
	return res;
}

bool step::is_step_active(){
	return flag_active;
}

void step::set_step_active(){
	flag_active = true;
}
void step::clr_step_active(){
	flag_active = false;
}

void step::set_next_step(step* s){
	_next = s;
}
step* step::get_next_step(){
	return _next;
}
void step::set_prev_step(step* s){
	_prev = s;
}
step* step::get_prev_step(){
	return _prev;
}

void step::set_step_color(uint8_t color){
	_color = color;
}
uint8_t step::get_step_color(){
	return _color;
}

uint8_t step::get_step_gate_len(){
	return gate_len_per;
}

void step::set_step_gate_len(uint32_t ms, uint8_t len){
	gate_len_per = len;
	gate_len_ms = ms;
}

void step::upd_step_gate_len(uint32_t ms_ref){
	gate_len_ms = ms_ref * gate_len_per / 100;
}

#define SERIALIZATION_BUFFER_MAX_SIZE		64
#define SERIALIZATION_TREE_START_BYTE_INDEX	1
#define SERIALIZATION_SUCCESS				0
static void set_bits(uint8_t *word, uint8_t from, uint8_t len){
	int i;
	for(i = from; i < from + len; i++) {
		*word |= (1 << i);
	}
}


void step::serialize_tree(step* s, struct serialized_tree_t* s_tree){
//	uint8_t buf[SERIALIZATION_BUFFER_MAX_SIZE];
//	int ret = SERIALIZATION_SUCCESS;
	step * next = s->get_next_step();
	step * cur = s;
	uint8_t bit_idx = s->_node->_node_depth % 16;
	uint8_t byte_idx = bit_idx / 16;
	memset(s_tree->buf, 0, SERIALIZATION_BUFFER_MAX_SIZE);

	// must be sure that node depth ≤ 8
	BIT::set_bits(&(s_tree->buf[SERIALIZATION_TREE_START_BYTE_INDEX]), 0, s->_node->_node_depth);
	
	while(next != s){
		// get node parent
		node* common_parent = node::get_common_parent(cur->_node, next->_node);
		// go up to current node depth minus common_parent depth
		uint8_t tmp = cur->_node->_node_depth - common_parent->_node_depth;
		byte_idx += (tmp + bit_idx) / 16;
		bit_idx = (bit_idx + tmp) % 16;

		// go down to next node depth minus common_parent_depth
		tmp = next->_node->_node_depth - common_parent->_node_depth;
		BIT::set_bits(&(s_tree->buf[byte_idx]), bit_idx, tmp);
		byte_idx += (tmp + bit_idx) / 16;
		bit_idx = (bit_idx + tmp) % 16;
		
		cur = next;
		next = s->get_next_step();
	}

	byte_idx += (8 + bit_idx) / 16;
	bit_idx = (bit_idx + 8) % 16;

	s_tree->nr_byte = byte_idx;
}

void step::deserialize_tree(node * root, struct serialized_tree_t * s_tree){
	node * node_ptr = root;
	step * prev_step = NULL;
	uint8_t byte_cnt = 0;
	bool break_flg = false;
	uint8_t i;
	for(i=0;i<s_tree->nr_byte;i+=2){
		uint16_t data = s_tree->buf[i];
		for(uint8_t j=0;j<16;j++){
			if(data & 0x1){
				// bit is set: create a node and attach to node_ptr children list
				node * n = new node;
				if(!node_ptr->_children){
					LinkedList<node *> *ll = new LinkedList<node *>;
					node_ptr->_children = ll;
				}
				node_ptr->_children->add(n);
				// do we need node_id
				// when n->_mtx_id should be set???

				// eventually create step if next bit = 0
				if( !(data & 0x2) ){
					step * s = new step;
					s->_node = n;
					n->_step = s;
					n->_node_is_step = true;
					if(prev_step){
						prev_step->set_next_step(s);
						s->set_prev_step(prev_step);
						prev_step = s;
					}
				}
			}
			else {
				// bit is not set: get parent node
				if(node_ptr == root){
					printf("root has been reached\n");
					break_flg = true;
				}
				else {
					node_ptr = node_ptr->_parent;
				}
			}
			data = data >> 1;
			if(break_flg)
				break;

		}
		if(break_flg)
			break;

	}
	if(break_flg && i != s_tree->nr_byte){
		printf("Warning: deserialization has been aborded before reaching last bytes...\n");
	}
}
