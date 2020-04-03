#include "fct_normalization.h"
#include "../errata.h"


#include <cstdlib>
#include <hw_debug.h>

#define LED_BLINKING_RATE_MS	200


void fct_normalization::init(sequencer* s, char* name){
    _seq = s;
    _norm_step = 0;
    track* t = _seq->get_current_track();
    fct_clbk::set_fct_name(name);
    step_blink_.initBlinking(t->get_led_matrix(), _norm_step,
                             LED_GBR_IDX, FOREGROUND1);
};

void fct_normalization::on_push(uint8_t btn_id){
    track* t = _seq->get_current_track();
    step *s = t->get_step_by_btn_id(btn_id);
    if(s){
        // be sure we are on a step
        // clr and restore previous led
//        track* t = _seq->get_current_track();
//        t->get_led_matrix()->clr_n_restore(_norm_step, FOREGROUND1);
        step_blink_.stopBlinking();
        // set led in white
        _norm_step = btn_id;
        step_blink_.setBlinkingLed(_norm_step);
        step_blink_.startBlinking(LED_BLINKING_RATE_MS);
    }
}

void fct_normalization::on_release(uint8_t btn_id){
    // call step
    track* t = _seq->get_current_track();
    step *s = t->get_step_by_btn_id(btn_id);
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
//    track* t = _seq->get_current_track();
    step_blink_.updateBlinking();
}

void fct_normalization::on_start(){
//    track* t = _seq->get_current_track();
//    t->get_led_matrix()->save_n_set(LED_GBR_IDX, _norm_step, FOREGROUND1);
    step_blink_.startBlinking(LED_BLINKING_RATE_MS);

}

void fct_normalization::on_leave(){
//    track* t = _seq->get_current_track();
//    t->get_led_matrix()->clr_n_restore(_norm_step, FOREGROUND1);
    step_blink_.stopBlinking();
}
