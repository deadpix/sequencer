#include "node.h"
#include <hw_debug.h>

node::node(){
	_parent = NULL;
	_step = NULL;
	_node_depth = 0;
	_mtx_id = 0;
	_children = NULL;
	_zombies = NULL;
	_node_is_step = false;
}
node::~node(){

	if(_children){
		for(int i=0; i<_children->size();i++){
			node* n = _children->remove(i);
			delete n;
		}
	}
	if(_zombies){
		for(int i=0; i<_zombies->size();i++){
			node* n = _zombies->remove(i);
			delete n;
		}
	}

	// delete step* ?
	if(_step){
		delete _step;
	}
	
	// delete parent?
	_parent = NULL;
}

void node::delete_tree(node * root){
	LinkedList<node *> *del_list = new LinkedList<node *>; 
	del_list->add(root);

	while(del_list->size() != 0){
		node * del = del_list->pop();
		if(del->_children){
			while(del->_children->size() != 0){
				del_list->add(del->_children->pop());
			}
		}
		if(del->_zombies){
			while(del->_zombies->size() != 0){
				del_list->add(del->_zombies->pop());
			}
		}
		delete del;
	}
	delete del_list;
}


node* node::get_node_depth(uint8_t depth){
	node* tmp = this;
	while(tmp->_node_depth > depth){
		tmp = tmp->_parent;
	}
	return tmp;
}

step* node::get_first_step(uint8_t max_depth){
	node* tmp = this;
	step* ret = NULL;
	for(uint8_t i=0; i<max_depth; i++){	
//		if(tmp->_step){
		if(tmp->_node_is_step){
			ret = tmp->_step;
			return ret;
		}
		else {
			tmp = tmp->_children->get(0);
			if(!tmp){
				dbg::println("problem in node::get_first_step function");
				break;
			}
		}
	}
	dbg::println("unrecoverable error in node::get_first_step");
	return ret;
}

step* node::get_last_step(uint8_t max_depth){
	node* tmp = this;
	step* ret = NULL;
	
	for(uint8_t i=0; i<max_depth; i++){
		if(tmp->_node_is_step){
			ret = tmp->_step;
			return ret;
		}
		else {
			tmp = tmp->_children->get(tmp->_children->size());
			if(!tmp){
				dbg::println("problem in node::get_last_step function");
				break;
			}
		}
	}
	dbg::println("unrecoverable error in node::get_last_step");
	return ret;
}

node* node::get_common_parent(node* n1, node* n2){
	node* tmp1 = n1;
	node* tmp2 = n2;


	if(tmp1->_node_depth > tmp2->_node_depth){
		tmp1 = tmp1->get_node_depth(tmp2->_node_depth);
	}
	else if(tmp1->_node_depth < tmp2->_node_depth){
		tmp2 = tmp2->get_node_depth(tmp1->_node_depth);
	}
	
	while(tmp1->_parent != tmp2->_parent){
		tmp1 = tmp1->_parent;
		tmp2 = tmp2->_parent;
	}
//	return tmp1->_parent;
	return tmp1->_parent;
}
