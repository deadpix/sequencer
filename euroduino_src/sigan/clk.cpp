#include "clk.h"

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
	_ms 		= bpm_to_ms(_bpm);
	_operation	= 1;
	_resync 	= false;
}

clk::~clk(){
}

void clk::clk_sync_extern(){
	_ms = _elapsed_ms;
	_elapsed_ms = 0;
	_bpm = ms_to_bpm(_ms);
	_step_cnt = (_step_cnt + 1) % _max_step;
}

void clk::clk_sync_intern(uint32_t ms){
	if(_operation < 0){
		_ms = ms * abs(_operation);
		_bpm = ms_to_bpm(_ms);
	} else {
		_ms = ms / _operation;
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
int clk::clk_get_operator(){
	return _operation;
}

void clk::clk_set_max_step(uint8_t max_step){
	_max_step = max_step;
}

int clk::clk_set_ms(uint32_t new_ms){
	int ret = 0;
	if(new_ms < _ms)
		ret = 1;
	_ms = new_ms;
	_bpm = ms_to_bpm(_ms);
	
	return ret;
}

int clk::clk_set_bpm(uint16_t new_bpm){
	int ret = 0;
	if(new_bpm > _bpm)
		ret = 1;
	_bpm = new_bpm;
	_ms = bpm_to_ms(_bpm);

	return ret;
}

boolean clk::clk_set_operation(int op, uint32_t ms_ref){
	boolean ret = true;

	if(op < 0){
		_ms = ms_ref * abs(op);
		_bpm = ms_to_bpm(_ms);
		_operation = op;
	} 
	else if(op > 0){
		_ms = ms_ref / op;
		_bpm = ms_to_bpm(_ms);
		_operation = op;
	}
	else if(op == 0){
		_operation = 1;
		ret = false;
	}
	if( (abs(op) > _max_step) || (_ms <= 0) ){
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

uint32_t clk::clk_sync_divider(uint32_t ms, uint16_t step){
	uint8_t divider = abs(_operation);
	uint32_t ret = 0;
	_ms = ms * divider;
	_bpm = ms_to_bpm(_ms);

	if( ((step +1) % divider) == 0 ){
		ret = _ms;
		_elapsed_ms = 0;
	}
	
	return ret;
}

uint32_t clk::clk_sync_multiplier(uint32_t ms){
	//TODO neeed to check step number for sync
	_ms = ms / _operation;
	_bpm = ms_to_bpm(_ms);
	_elapsed_ms = 0;

//	_resync = true;
	return _ms;
}

uint32_t clk::clk_reset(){
	_elapsed_ms = 0;
	_step_cnt = 0;
	return _ms;
}

void clk::clk_sync_slaved(uint16_t step){
	if(_operation < 0){
		if( (step % abs(_operation)) == 0 )
			_resync = true;
	}
	else {
		_resync = true;
	}
}

uint32_t clk::clk_sync(uint32_t ms, uint16_t step){
	uint32_t ret;
	if(_operation < 0){
		ret = clk_sync_divider(ms, step);
	}
	else {
		ret = clk_sync_multiplier(ms);
	}
	return ret;
}

uint32_t clk::clk_elapsed(){
	uint32_t ret = 0;
	if(_elapsed_ms > _ms){
		_elapsed_ms = 0;
		_step_cnt = (_step_cnt + 1) % _max_step;
		ret = _ms;
	}
	return ret;
}

uint32_t clk::master_sync(uint32_t mst_ms, uint16_t mst_cnt){
	uint32_t ret = 0;

	/* master has been synced or received new tick */
	if(mst_ms > 0){
		ret = clk_sync(mst_ms, mst_cnt);
	}
	/* "multiplied" slave clk sync itself until _step_cnt = _operation     */
	/* when _step_cnt = _operation, slave clk has to be synced with master */
	else if( (_operation > 0) && (_step_cnt < (_operation - 1)) ){
		ret = clk_elapsed(); 
	}
	
	return ret;
}

boolean clk::clk_update(boolean sync_flg, clk* master){
	uint8_t divider = abs(_operation);	
	boolean ret = false;
	uint32_t ms = master->clk_get_ms();
	uint16_t step = master->clk_get_step_cnt();
	
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
			_ms = ms / _operation;
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
