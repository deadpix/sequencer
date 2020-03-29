#include "fct_normalization.h"
#include "../errata.h"


#include <cstdlib>
#include <hw_debug.h>

void fct_normalization::init(sequencer* s, char* name){
    _seq = s;
    _norm_step = 0;
    fct_clbk::set_fct_name(name);
};

void fct_normalization::on_push(uint8_t btn_id){
    track* t = _seq->get_current_track();
    if(t->_mtx_to_node[btn_id]->_step){
        // be sure we are on a step
        // clr and restore previous led
        track* t = _seq->get_current_track();
        t->get_led_matrix()->clr_n_restore(_norm_step, FOREGROUND1);

        // set led in white
        _norm_step = btn_id;
        t->get_led_matrix()->save_n_set(LED_GBR_IDX, _norm_step, FOREGROUND1);
    }
}

void fct_normalization::on_release(uint8_t btn_id){
    // call step
    track* t = _seq->get_current_track();
    step * s = t->_mtx_to_node[btn_id]->_step;
    if(s){
        s->normalize_steps(s->_clk_def.numerator, s->_clk_def.denominator);
    }

}

void fct_normalization::on_long_release(uint8_t btn_id){
    UNUSED(btn_id);
}

void fct_normalization::update_ui(uint32_t mst_ms, uint16_t mst_step){
    UNUSED(mst_ms);
    UNUSED(mst_step);
}

void fct_normalization::on_start(){
    track* t = _seq->get_current_track();
    t->get_led_matrix()->save_n_set(LED_GBR_IDX, _norm_step, FOREGROUND1);
}

void fct_normalization::on_leave(){
    track* t = _seq->get_current_track();
    t->get_led_matrix()->clr_n_restore(_norm_step, FOREGROUND1);
}
