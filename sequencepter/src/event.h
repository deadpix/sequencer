#ifndef __SRC_EVENT_H__
#define __SRC_EVENT_H__

enum evt_type {
	EVT_MASTER_TICK = 0,
	EVT_NEXT_STEP,
	NR_EVT
};

class event {
	private:
		evt_type _evt_id;
	
	public:
		void set_event_id(evt_type id){ _evt_id = id; };
		evt_type get_event_id(){return _evt_id;};
		virtual void do_evt() = 0;
};

#endif
