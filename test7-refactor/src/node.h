#ifndef __NODE_H__
#define __NODE_H__

#include <stdint.h>
#include <LinkedList.h>
#include "types.h"

class node;

#include "step.h"

class node {
	public:
		LinkedList<node *>* _children;
		node* _parent;
		step* _step;
		uint8_t	_node_lvl;
		uint8_t _mtx_id;
		uint8_t _node_id;

		node();
		~node();		

		node* get_node_lvl(uint8_t lvl);
		step* get_first_step(uint8_t max_lvl);
		static node* get_common_parent(node*, node*);	
};

#endif
