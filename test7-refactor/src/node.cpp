#include "node.h"

node::node(){
	_parent = NULL;
	_step = NULL;
	_node_lvl = 0;
	_mtx_id = 0;
	_children = NULL;
}
node::~node(){
	for(int i=0; i<_children->size();i++){
		node* n = _children->remove(i);
		delete n;
	}
	// delete step* ?
	if(_step){
		delete _step;
	}
	
	// delete parent?
	_parent = NULL;
}

node* node::get_node_lvl(uint8_t lvl){
	node* tmp = _parent;
	while(_parent->_node_lvl < lvl){
		tmp = tmp->_parent;
	}
	return tmp;
}

node* node::get_common_parent(node* n1, node* n2){
	node* tmp1 = n1;
	node* tmp2 = n2;
	
	while(tmp1->_parent != tmp2->_parent){
		tmp1 = tmp1->_parent;
		tmp2 = tmp2->_parent;
	}
	return tmp1->_parent;
}
