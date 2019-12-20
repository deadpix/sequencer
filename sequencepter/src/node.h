#ifndef __NODE_H__
#define __NODE_H__

#include <stdint.h>
#include <LinkedList.h>
#include "types.h"

class node;

#include "step.h"

#define NODE_TREE_MAX_DEPTH(nr_steps)	(nr_steps/MATRIX_NR_COL)

class node {
	public:
		LinkedList<node *>* _children;
		LinkedList<node *>* _zombies;
		node* _parent;
		step* _step;
		uint8_t	_node_depth;
		uint8_t _mtx_id; 	// id of the led / button matrix id
		uint8_t _node_id;	// id of the node in a sub-sequence 
		bool _node_is_step;

		node();
		~node();		
//		void delete_tree(node * root);
        void delete_tree();

		node* get_node_depth(uint8_t depth);
		step* get_first_step(uint8_t max_depth);
		step* get_last_step(uint8_t max_depth);
		static node* get_common_parent(node*, node*);	
};

#endif
