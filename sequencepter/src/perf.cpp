#include "perf.h"
#include <hw_debug.h>

perf::perf(){
	_ms_min = 0xffffffff;
	_ms_max = 0;
	_ms_cum = 0;
	_cnt = 0;
	_ovf = false;
}

void perf::start_ms_counter(){
	_ms_cnt = 0;
}

void perf::stop_ms_counter(){
	uint32_t tmp = _ms_cnt;
	_ms_cum += tmp;
	if(_ms_cum > (UINT_MAX - tmp))
		_ovf = true;
	_cnt++; 
	if(tmp < _ms_min)
		_ms_min = tmp;
	if(tmp > _ms_max)
		_ms_max = tmp;
}

void perf::reset_ms_counter(){
	_ms_min = 0xffffffff;
	_ms_max = 0;
	_ms_cum = 0;
	_cnt = 0;
	_ovf = false;
}

void perf::print_perf(){
	dbg::print("Perf in ms for ");
	dbg::print(_cnt);
	dbg::println(" measures:");
	dbg::print(" - min: ");
	dbg::println(_ms_min);
	dbg::print(" - max: ");
	dbg::println(_ms_max);
	dbg::print(" - avg: ");
	if(!_ovf)
		dbg::println(_ms_cum / _cnt);
	else
		dbg::println(" overflow");
}
