#ifndef __SRC_SEQUENCER_FCT_NORMALIZATION_H__
#define __SRC_SEQUENCER_FCT_NORMALIZATION_H__

#include <stdint.h>


#include "../fct_clbk.h"
#include "../sequencer.h"
#include "../led_toogle.h"
#include "../blink.h"

class fct_normalization: public fct_clbk {
    private:
        sequencer*  _seq;
        uint8_t     _norm_step;
        blink       step_blink_;

    public:
        fct_normalization(){}
        ~fct_normalization(){}

        void init(sequencer*, char* name);
        void on_push(uint8_t btn_id);
        void on_long_push(uint8_t btn_id){ UNUSED(btn_id); }
        void on_release(uint8_t btn_id);
        void on_long_release(uint8_t btn_id);
        void update_ui(uint32_t mst_ms, uint16_t mst_step);
        void on_start();
        void on_leave();
};


#endif // __SRC_SEQUENCER_FCT_NORMALIZATION_H__
