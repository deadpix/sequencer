#ifndef __FCT_CLBK_H__
#define __FCT_CLBK_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

class fct_clbk {
	private:
		char* _fct_name;
	
	public:
		char* get_fct_name();
		void  set_fct_name(char* str);
		
		virtual void on_push(uint8_t btn_id) = 0;
		virtual void on_long_push(uint8_t btn_id) = 0;
		virtual void on_release(uint8_t btn_id) = 0;
		virtual void update_ui(uint32_t mst_ms, uint16_t mst_step) = 0;
		
		virtual void on_start() = 0;
		virtual void on_leave() = 0;
};

#endif
