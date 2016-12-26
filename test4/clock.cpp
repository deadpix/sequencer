#include "clock.h"
#include "configuration.h"

#define MAX_DIVIDER 	16
#define DELAY_MS		50

static uint32_t bpm_to_ms(uint16_t bpm){
	return (60000/bpm);
}

static uint16_t ms_to_bpm(uint32_t ms){
	return (uint16_t)(ms/60000);
}

static uint32_t reset_gate_ms(uint32_t inter_ms, uint8_t len){
	return (inter_ms * len) / 100;
}

clock::clock(){
	step		= 0;
	bpm 		= 180;
	elapsed_ms 	= 0;
	gate_len 	= 5;
	interval_ms = bpm_to_ms(bpm);
	gate_ms		= (interval_ms * gate_len) / 100;
	factor		= 1;
	resync 		= false;
}

clock::~clock(){
}

void clock::sync_extern(){
	interval_ms = elapsed_ms;
	elapsed_ms 	= 0;
	bpm = ms_to_bpm(interval_ms);
	gate_ms	= (interval_ms * gate_len) / 100;
	step = (step + 1) % NR_STEP;
}

void clock::sync_intern(uint32_t ms){
	if(factor < 0){
		interval_ms = ms * abs(factor);
		bpm = ms_to_bpm(interval_ms);
		gate_ms = reset_gate_ms(interval_ms, gate_len);
	} else {
		interval_ms = ms / factor;
		bpm = ms_to_bpm(interval_ms);
		gate_ms = reset_gate_ms(interval_ms, gate_len);
	}
}

uint32_t clock::getInterval(){
	return interval_ms;
}
uint16_t clock::getStep(){
	return step;
}
uint32_t clock::getGateLen(){
	return gate_ms;
}
uint32_t clock::getElapsedMs(){
	return elapsed_ms;
 }

void clock::setFactor(int f){
	factor = f;
	if(f < 0){
		bpm /= abs(f);
		interval_ms = bpm_to_ms(bpm);
		gate_ms = reset_gate_ms(interval_ms, gate_len);
	} else {
		bpm *= f;
		interval_ms = bpm_to_ms(bpm);
		gate_ms = reset_gate_ms(interval_ms, gate_len);
	}
}

boolean clock::clock_next_step(boolean* gate_status){

	if(resync){
		resync = false;
		elapsed_ms = 0;
		step = (step + 1) % NR_STEP;
		*gate_status = true;
		return true;
	}
	else if(elapsed_ms > interval_ms){
		elapsed_ms = 0;
		step = (step + 1) % NR_STEP;
		*gate_status = true;
		return true;
	}
	else if(elapsed_ms < gate_ms){
		*gate_status = true;
		return false;
	} else {
		*gate_status = false;
		return false;
	}
} 

boolean clock::clock_get_gate_status(){
	if(elapsed_ms < gate_ms)
		return true;
	else 
		return false;
}

uint8_t clock::clock_get_gate_status2(){
	if(elapsed_ms < gate_ms)
		return 1;
	if((elapsed_ms >= gate_ms) && (elapsed_ms < (gate_ms+DELAY_MS)))
		return 2;
	else 
		return 0;
}

void clock::clock_sync_divider(uint32_t ms, uint16_t step){
	uint8_t divide = abs(factor);
//	interval_ms = ms / divide;
	sync_intern(ms);
	if( (step % divide) == 0 )
		resync = true;
}

void clock::clock_sync_multiplier(uint32_t ms){
	//TODO neeed to check step number for sync
	sync_intern(ms);
	resync = true;
}

boolean clock::clock_update(boolean sync_flg, clock* master){
	uint8_t divide = abs(factor);	
	boolean ret = false;
	uint32_t ms = master->getInterval();
	uint16_t step = master->getStep();
	
	/* sync case */
	if(sync_flg){
		if(factor < 0){
			interval_ms = ms * abs(factor);
			bpm = ms_to_bpm(interval_ms);
//			gate_ms = reset_gate_ms(interval_ms, gate_len);
			if( (step % divide) == 0 ){
				elapsed_ms = 0;
				step = (step + 1) % NR_STEP;
				ret = true;
			}
		} else {
			interval_ms = ms / factor;
			bpm = ms_to_bpm(interval_ms);
//			gate_ms = reset_gate_ms(interval_ms, gate_len);
			elapsed_ms = 0;
			step = (step + 1) % NR_STEP;
			ret = true;
		}
	}
	else if(elapsed_ms > interval_ms){
		elapsed_ms = 0;
		step = (step + 1) % NR_STEP;
		ret = true;
	}
	return ret;
}

void clock::clock_upd_gui(uint8_t step, led_matrix *lm, /*boolean clk_state,*/ boolean pixel_state){
//	gui_state = get_pixel_status(step);
//	if(clk_state){
//		lm->set_pixel(curr_step);
//	}
//	else {
		// get led gate len
		uint8_t clk_gate_status = clock_get_gate_status2();
		if(clk_gate_status == 0){
			// restore original pixel state
			lm->set_pixel_status(step, pixel_state);
		}
		else if(clk_gate_status == 1){
			lm->set_pixel(step);
		} 
		else if(clk_gate_status == 2){
			lm->clr_pixel(step);			
		}
//	}
}