#ifndef __NODE_H__
#define __NODE_H__

#include <stdint.h>
#include <LinkedList.h>
#include "types.h"
#include "step.h"

class node {
	public:
		LinkedList<node *>* _children;
		node* _parent;
		step* _s;
		uint8_t	_node_lvl;
		uint8_t _mtx_id;

		node();
		~node();		

		node* get_node_lvl(uint8_t lvl);
		static node* get_common_parent(node*, node*);	
};

#endif
