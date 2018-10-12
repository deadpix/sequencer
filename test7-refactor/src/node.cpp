#include "node.h"
#include <hw_debug.h>

node::node(){
	_parent = NULL;
	_step = NULL;
	_node_lvl = 0;
	_mtx_id = 0;
	_children = NULL;
}
node::~node(){

	if(_children){
		for(int i=0; i<_children->size();i++){
			node* n = _children->remove(i);
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

node* node::get_node_lvl(uint8_t lvl){
	node* tmp = this;
	while(tmp->_node_lvl < lvl){
		tmp = tmp->_parent;
	}
	return tmp;
}

step* node::get_first_step(uint8_t max_lvl){
	node* tmp = this;
	step* ret = NULL;
	for(uint8_t i=0; i<max_lvl; i++){	
		if(tmp->_step){
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

node* node::get_common_parent(node* n1, node* n2){
	node* tmp1 = n1;
	node* tmp2 = n2;

	if(tmp1->_node_lvl > tmp2->_node_lvl){
		tmp1 = tmp1->get_node_lvl(tmp2->_node_lvl);
	}
	else if(tmp1->_node_lvl < tmp2->_node_lvl){
		tmp2 = tmp2->get_node_lvl(tmp1->_node_lvl);
	}
	
	while(tmp1->_parent != tmp2->_parent){
		tmp1 = tmp1->_parent;
		tmp2 = tmp2->_parent;
	}
	return tmp1->_parent;
}
