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

#define SERIALIZATION_HEADER_MAGIC_BYTE_OFFSET      0
#define SERIALIZATION_HEADER_NR_NODES_BYTE_OFFSET   1
#define SERIALIZATION_HEADER_NR_STEPS_BYTE_OFFSET   3
#define SERIALIZATION_HEADER_TREE_SZ_BYTE_OFFSET    5
#define SERIALIZATION_HEADER_BYTE_SIZE              7

#define SERIALIZATION_ERROR                         1
#define SERIALIZATION_OK                            0


struct serialization_header_t {
    uint8_t magic;
    uint16_t nr_nodes;
    uint16_t nr_steps;
    uint16_t tree_byte_sz;
};

#define SERIALIZATION_DATA_GATE_LEN_OFFSET  0
#define SERIALIZATION_DATA_VELOCITY_OFFSET  sizeof(uint8_t)
#define SERIALIZATION_DATA_PITCH_OFFSET     SERIALIZATION_DATA_VELOCITY_OFFSET + sizeof(uint8_t)
#define SERIALIZATION_DATA_CLK_OFFSET       SERIALIZATION_DATA_PITCH_OFFSET + sizeof(uint16_t)
#define SERIALIZATION_DATA_COLOR_OFFSET     SERIALIZATION_DATA_CLK_OFFSET + sizeof(struct clk_def)
#define SERIALIZATION_DATA_MTX_ID_OFFSET    SERIALIZATION_DATA_COLOR_OFFSET + sizeof(uint8_t)

struct __attribute__((__packed__)) serialization_data_t {
    uint8_t gate_len;
    uint8_t velocity;
    uint16_t pitch;
    struct clk_def clk;
    uint8_t color;
    uint8_t mtx_id;
};

struct serialized_tree_t {
    struct serialization_header_t header;
    uint8_t * buf;
    const uint16_t nr_byte;
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
        void set_step_gate_len_per(uint8_t per){ gate_len_per = per; }
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
        void normalize_steps(uint8_t num, uint8_t denom);

        static int serialize_tree(step* s, node * root, struct serialized_tree_t* s_tree, uint16_t * serialized_data_sz);
        static int deserialize_tree(node ** root, struct serialized_tree_t * s_tree, step ** first, step ** last);
};

#endif
