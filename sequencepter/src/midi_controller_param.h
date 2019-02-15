#ifndef __SRC_MIDI_CTRL_PARAM_H__
#define __SRC_MIDI_CTRL_PARAM_H__

#include "midi_controller.h"

class midi_controller_param: public param {
	private:
		midi_controller* mc_;
		struct midi_controller_conf* last_conf_;
	
		void redraw_midi_controller_param();
	
	public:
		midi_controller_param(){};
		~midi_controller_param(){};

		void init(midi_controller* mc);

		void on_push(uint8_t btn_id);
		void on_long_push(uint8_t btn_id){ UNUSED(btn_id); };
		void on_release(uint8_t btn_id);
		void on_long_release(uint8_t btn_id){ UNUSED(btn_id); };
		void update_ui(uint32_t mst_ms, uint16_t mst_step);

		void param_on_enter();
		void param_on_leave();
};
#endif
