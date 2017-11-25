#ifndef __SEQ_PARAM_H__
#define __SEQ_PARAM_H__

#include "param.h"
#include "sequencer.h"
#include "track.h"


class seq_param: public param {
	private:
		sequencer* _s;
		
	public:
		seq_param();
		~seq_param();
		
		void init(sequencer* const s);
		
		void on_push(uint8_t btn_id);
		void on_release(uint8_t btn_id);
		void update_ui();
};

#endif