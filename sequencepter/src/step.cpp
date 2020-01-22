#include "step.h"
#include "led_matrix.h"
#include <hw_debug.h>
#include "bit.h"
#include "types.h"

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

#define SERIALIZATION_SUCCESS				0
#define SERIALIZATION_ERR_MEM_SPACE			1
#define SERIALIZATION_BIT_PER_BYTE			8
#define SERIALIZATION_MAGIC                 0x1

static void set_post_order_list(LinkedList<node *> * list, node * parent, node * child){
    node * n = child->_parent;
    while(n != parent){
        list->add(n);
        n = n->_parent;
    }
}
static void set_pre_order_list(LinkedList<node *> * from, LinkedList<node *> * to){
    int list_sz = from->size();
    for(int i=0; i<list_sz;i++){
        to->add(from->pop());
    }
    // Assert
    //    ASSERT(from->size() == 0);
    if(from->size() > 0)
        dbg::printf("Problem: from list is not empty...\n");
}

static void save_node_data(struct serialization_data_t * sd, node * n){
    if(n->_node_is_step){
        sd->gate_len = n->_step->get_step_gate_len();
        sd->velocity = n->_step->_note.velocity;
        sd->pitch = n->_step->_note.pitch;
        sd->clk.numerator = n->_step->_clk_def.numerator;
        sd->clk.denominator = n->_step->_clk_def.denominator;
        sd->color = n->_step->get_step_color();
        sd->mtx_id = n->_mtx_id;
    }
    else {
        sd->gate_len = 0;
        sd->velocity = 0;
        sd->pitch = 0;
        sd->clk.numerator = 0;
        sd->clk.denominator = 0;
        sd->color = 0;
        sd->mtx_id = n->_mtx_id;
    }
}

static void dump_node_list(LinkedList<node *> *nl){
    for(int i=0;i<nl->size();i++){
        node * n = nl->get(i);
        dbg::printf("node_id=%d depth=%d\n",n->_node_id, n->_node_depth);
    }
}
static void deserialization_get_node_mtx_id(uint8_t* data_buf, uint16_t data_offset, uint8_t* mtx_id){
    *mtx_id = data_buf[data_offset+SERIALIZATION_DATA_MTX_ID_OFFSET];
}
static void deserialization_get_step_data(uint8_t* data_buf, uint16_t data_offset, step* s){
    uint16_t tmp = data_offset+SERIALIZATION_DATA_GATE_LEN_OFFSET;
    s->set_step_gate_len_per(data_buf[tmp]);

    tmp = data_offset+SERIALIZATION_DATA_PITCH_OFFSET;
    uint16_t tmp_pitch = (data_buf[tmp+1] << 8) | data_buf[tmp];
    tmp = data_offset+SERIALIZATION_DATA_VELOCITY_OFFSET;
    s->step_set_note(data_buf[tmp], tmp_pitch);

    tmp = data_offset+SERIALIZATION_DATA_COLOR_OFFSET;
    s->set_step_color(data_buf[tmp]);

    tmp = data_offset+SERIALIZATION_DATA_CLK_OFFSET;
    s->_clk_def.numerator = data_buf[tmp];

    tmp = data_offset+SERIALIZATION_DATA_CLK_OFFSET+sizeof(uint8_t);
    s->_clk_def.denominator = data_buf[tmp];
}

static void init_step_from_serialized_data(step * s, struct serialization_data_t * data_ptr){
    s->set_step_gate_len_per(data_ptr->gate_len);
    s->step_set_note(data_ptr->velocity, data_ptr->pitch);
    s->_clk_def.numerator = data_ptr->clk.numerator;
    s->_clk_def.denominator = data_ptr->clk.denominator;
    s->set_step_color(data_ptr->color);
}

int step::serialize_tree(step* s, node * root, struct serialized_tree_t* s_tree,
                         uint16_t * serialized_data_sz){
    int ret = SERIALIZATION_SUCCESS;
	step * next = s->get_next_step();
	step * cur = s;
    LinkedList<node *> *tmp_list = new LinkedList<node *>;
    LinkedList<node *> *serialization_list = new LinkedList<node *>;
    *serialized_data_sz = 0;


    uint16_t nr_steps = 0; // needed????

    uint8_t bit_idx = (s->_node->_node_depth+1) % SERIALIZATION_BIT_PER_BYTE;
    uint16_t byte_idx = bit_idx / SERIALIZATION_BIT_PER_BYTE;
    memset(s_tree->buf, 0, s_tree->nr_byte);

    // must be sure that node depth ≤ 8
    BIT::set_bits(s_tree->buf, 0, s->_node->_node_depth+1);
    uint16_t nr_nodes = s->_node->_node_depth+1;

    set_post_order_list(tmp_list, root, s->_node);
    serialization_list->add(root);
    set_pre_order_list(tmp_list, serialization_list);
    serialization_list->add(s->_node);

	while(next != s){
        // verify that remaining memory space
        if(byte_idx >= s_tree->nr_byte){
            ret = SERIALIZATION_ERR_MEM_SPACE;
            break;
        }

		// get node parent
		node* common_parent = node::get_common_parent(cur->_node, next->_node);

        // go up to current node depth minus common_parent depth
		uint8_t tmp = cur->_node->_node_depth - common_parent->_node_depth;
        byte_idx += (tmp + bit_idx) / SERIALIZATION_BIT_PER_BYTE;
        bit_idx = (bit_idx + tmp) % SERIALIZATION_BIT_PER_BYTE;

		// go down to next node depth minus common_parent_depth
		tmp = next->_node->_node_depth - common_parent->_node_depth;
		BIT::set_bits(&(s_tree->buf[byte_idx]), bit_idx, tmp);
        byte_idx += (tmp + bit_idx) / SERIALIZATION_BIT_PER_BYTE;
        bit_idx = (bit_idx + tmp) % SERIALIZATION_BIT_PER_BYTE;
        nr_nodes += tmp;

        // fill the serializaztion list
        set_post_order_list(tmp_list, common_parent, next->_node);
        set_pre_order_list(tmp_list, serialization_list);
        serialization_list->add(next->_node);


		cur = next;
        next = cur->get_next_step();

        nr_steps++;
	}

    if(!ret){
        // is it needed???
        // last step is reached
        if(bit_idx == 0 || (bit_idx + cur->_node->_node_depth) > SERIALIZATION_BIT_PER_BYTE)
            ++byte_idx;

        s_tree->header.tree_byte_sz = byte_idx;
        s_tree->header.magic = SERIALIZATION_MAGIC;
        s_tree->header.nr_nodes = nr_nodes;
        s_tree->header.nr_steps = nr_steps + 1;

        // serialize data for each nodes in the serialization list
        for(int i=0;i<serialization_list->size();i++){

            if( (byte_idx + sizeof(struct serialization_data_t)) >= s_tree->nr_byte){
                ret = SERIALIZATION_ERR_MEM_SPACE;
                break;
            }

            struct serialization_data_t tmp;
            uint8_t * ptr = (uint8_t *) &tmp;
            save_node_data(&tmp, serialization_list->get(i));
            for(int j=0;j<sizeof(struct serialization_data_t);j++){
                s_tree->buf[byte_idx+j] = ptr[j];
            }
            byte_idx += sizeof(struct serialization_data_t);
        }

        dump_node_list(serialization_list);
        if(tmp_list->size() > 0){
            dbg::printf("tmp list not null... problem in algo\n");
        }
    }
    delete tmp_list;
    delete serialization_list;

    if(ret){
        dbg::printf("%s serialization error: %d\n",__FUNCTION__,ret);
    }
    else {
//        *serialized_data_sz = byte_idx + sizeof(serialization_header_t);
        *serialized_data_sz = byte_idx;
        dbg::printf("serialization done: size %d bytes\n",byte_idx+sizeof(serialization_header_t));
    }

    return ret;
}


int step::deserialize_tree(node ** root, struct serialized_tree_t * s_tree, step ** first, step ** last){
    node * node_ptr = new node;
    *root = node_ptr;
    step * prev_step = NULL;
	bool break_flg = false;
    uint8_t i, depth_cnt;
    uint16_t node_cnt = 0, step_cnt = 0;
    uint16_t tree_buffer_offset = s_tree->header.tree_byte_sz;


    __attribute__((packed)) const struct serialization_data_t * serialized_data = (struct serialization_data_t *) s_tree->buf + s_tree->header.tree_byte_sz;
    __attribute__((packed)) struct serialization_data_t * data_ptr;

    LinkedList<node *> *tNodeList = new LinkedList<node *>;

    int ret = SERIALIZATION_OK;

    for(i=0;i<s_tree->header.tree_byte_sz;i++){
        uint8_t data = s_tree->buf[i];

        if(i == 0 && !(data & 0x1)){
            dbg::printf("Sanity check at %s fails: first bit of serialized tree 0x%x should be set...",__FUNCTION__,data);
            return SERIALIZATION_ERROR;
        }

        dbg::printf("byte: %d\n",data);
        for(uint8_t j=0;j<SERIALIZATION_BIT_PER_BYTE;j++){
            uint8_t next_bit;
            if(j == (SERIALIZATION_BIT_PER_BYTE - 1)){
                if(i < (s_tree->header.tree_byte_sz-1)){
                    next_bit = s_tree->buf[i+1] & 0x1;
                }
                else {
                    dbg::printf("Warning: cannot get next bit in %s \n",__FUNCTION__);
                    next_bit = 1;
                }
            }
            else {
                next_bit = (data >> 1) & 0x1;
            }


            if(data & 0x1){
                node * n;
                if(i == 0 && j == 0){
                    n = node_ptr;
                    depth_cnt = 0;
                }
                else {
                    n = new node;
                    tNodeList->add(n);
                    ++depth_cnt;
                }
                tree_buffer_offset = (s_tree->header.tree_byte_sz + node_cnt * sizeof(struct serialization_data_t));
//                data_ptr = (struct serialization_data_t *)(serialized_data + node_cnt * sizeof(struct serialization_data_t));
                deserialization_get_node_mtx_id(s_tree->buf, tree_buffer_offset, &n->_mtx_id);
//                tree_buffer_offset += sizeof(struct serialization_data_t);
                //                n->_mtx_id = data_ptr->mtx_id;
				if(!node_ptr->_children){
					LinkedList<node *> *ll = new LinkedList<node *>;
					node_ptr->_children = ll;
				}
                if(node_ptr != n)
                    node_ptr->_children->add(n);
                ++node_cnt;

                n->_node_depth = depth_cnt;
				// do we need node_id
				// when n->_mtx_id should be set???

				// eventually create step if next bit = 0
//				if( !(data & 0x2) ){
                if( !next_bit ){
                    step * s = new step;
					s->_node = n;
					n->_step = s;
					n->_node_is_step = true;

                    deserialization_get_step_data(s_tree->buf, tree_buffer_offset,s);

                    if(prev_step){
						prev_step->set_next_step(s);
						s->set_prev_step(prev_step);
					}
                    prev_step = s;
                    if(step_cnt == 0) *first = s;
                    *last = s;
                    dbg::printf("first=%x last=%x\n",*first,*last);
                    ++step_cnt;
				}
                if(n != node_ptr){
                    n->_parent = node_ptr;
                    node_ptr = n;
                }
                else {
                    n->_parent = NULL;
                }
			}
			else {
//                node_ptr = node_ptr->_parent;

                --depth_cnt;
				// bit is not set: get parent node
                if(node_ptr == *root){
                    dbg::printf("root has been reached\n");
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
    if(break_flg && i != s_tree->header.tree_byte_sz-1){
        dbg::printf("Warning: deserialization has been abort (%d bytes) before reaching last bytes %d...\n",i,s_tree->header.tree_byte_sz);
        ret = SERIALIZATION_ERROR;
	}
    if( node_cnt != s_tree->header.nr_nodes){
        dbg::printf("error in deserialization: %d nodes has been counted but %d nodes in header",
               node_cnt, s_tree->header.nr_nodes);
        ret = SERIALIZATION_ERROR;
    }
    if( step_cnt != s_tree->header.nr_steps){
        dbg::printf("error in deserialization: %d steps has been counted but %d steps in header",
               step_cnt, s_tree->header.nr_steps);
        ret = SERIALIZATION_ERROR;
    }
    if(ret != SERIALIZATION_OK){
        dbg::printf("error = %d\n",ret);
        int sz = tNodeList->size();
        for(int i=0;i<sz;i++){
            node * n = tNodeList->pop();
            delete n;
        }
        delete tNodeList;
        *root = NULL;
    }
    return ret;
}
