#include "clock.h"

#define MAX_DIVIDER 	16
#define DELAY_MS		50

static uint32_t bpm_to_ms(uint16_t bpm){
	return (60000/bpm);
}

static uint16_t ms_to_bpm(uint32_t ms){
	return (uint16_t)(ms/60000);
}

clk::clk(){
	_step_cnt	= 0;
	_max_step	= 16;
	_bpm 		= 90;
	_elapsed_ms = 0;
	_ms 		= bpm_to_ms(bpm);
	_operation	= 1;
	_resync 	= false;
}

clk::~clk(){
}

void clk::clk_sync_extern(){
	_ms = _elapsed_ms;
	_elapsed_ms = 0;
	_bpm = ms_to_bpm(_ms);
	_step_cnt = (step_cnt + 1) % _max_step;
}

void clk::_sync_intern(uint32_t ms){
	if(_operation < 0){
		_ms = ms * abs(_operation);
		_bpm = ms_to_bpm(_ms);
	} else {
		_ms = ms / factor;
		_bpm = ms_to_bpm(_ms);
	}
}

uint32_t clk::clk_get_ms(){
	return _ms;
}
uint16_t clk::clk_get_step_cnt(){
	return _step_cnt;
}
uint32_t clk::clk_get_elapsed_ms(){
	return _elapsed_ms;
}
uint32_t clk::clk_set_max_step(uint8_t max_step){
	_max_step = max_step;
}

boolean clk::clk_set_operation(int op){
	boolean ret = true;
	if(op < 0){
		_bpm /= abs(op);
		_ms = bpm_to_ms(_bpm);
		_operation = op;
	} 
	else if(op > 0){
		_bpm *= op;
		_ms = bpm_to_ms(_bpm);
		_operation = op;
	}
	else if(op == 0){
		_operation = 1;
		ret = false;
	}
	if( abs(op) > _max_step ){
		ret = false;
	}
	return ret;
}

boolean clk::clk_next_step(){
	boolean ret = false;
	if(_resync){
		_resync = false;
		_elapsed_ms = 0;
		_step_cnt = (_step_cnt + 1) % _max_step;
		ret = true;
	}
	else if(_elapsed_ms > _ms){
		_elapsed_ms = 0;
		_step_cnt = (_step_cnt + 1) % _max_step;
		ret = true;
	}
	return ret;
} 

void clk::clk_sync_divider(uint32_t ms, uint16_t step){
	uint8_t divider = abs(_operation);
//	_ms = ms / divide;
	sync_intern(ms);
	if( (step % divider) == 0 )
		_resync = true;
}

void clk::clk_sync_multiplier(uint32_t ms){
	//TODO neeed to check step number for sync
	sync_intern(ms);
	resync = true;
}

boolean clk::clk_update(boolean sync_flg, clock* master){
	uint8_t divider = abs(_operation);	
	boolean ret = false;
	uint32_t ms = master->clk_get_ms();
	uint16_t step = master->get_step_cnt();
	
	// sync forced by master
	if(sync_flg){
		if(_operation < 0){
			_ms = ms * divider;
			_bpm = ms_to_bpm(_ms);
			if( (step % divider) == 0 ){
				_elapsed_ms = 0;
				_step_cnt = (_step_cnt + 1) % _max_step;
				ret = true;
			}
		} else {
			_ms = ms / factor;
			_bpm = ms_to_bpm(_ms);
			_elapsed_ms = 0;
			_step_cnt = (_step_cnt + 1) % _max_step;
			ret = true;
		}
	}
	// or sync because of timeup
	else if(_elapsed_ms > _ms){
		_elapsed_ms = 0;
		_step_cnt = (_step_cnt + 1) % _max_step;
		ret = true;
	}
	return ret;
}
