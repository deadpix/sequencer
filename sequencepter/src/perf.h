#ifndef __SRC_PERF_H__
#define __SRC_PERF_H__

#include <stdint.h>
#include <elapsedMillis.h>
#include <limits.h>

#define BUFFER_SIZE	512

class perf {
	private:
		elapsedMillis _ms_cnt;
		uint32_t _ms_min;
		uint32_t _ms_max;
		uint32_t _ms_cum;	
		uint32_t _cnt;
		bool _ovf;

	public:
		perf();
		~perf(){};
		void start_ms_counter();
		void stop_ms_counter();
		void reset_ms_counter();
		void print_perf();
		uint32_t get_perf_cnt(){ return _cnt;};
};

#endif
