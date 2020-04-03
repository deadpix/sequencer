#ifndef __BLINK_H__
#define __BLINK_H__

#include <elapsedMillis.h>
#include "led_matrix.h"

#define STARTED			true
#define STOPPED			false


class blink {
    private:
        elapsedMillis   timeCnt_;
        led_matrix*     lm_;
        bool            state_ = STOPPED;
        uint16_t        led_id_;
        uint8_t         color_;
        uint8_t         ground_;
        uint32_t        blinkingTime_;
        bool            turnedOnFlag = false;

    public:
        void initBlinking( led_matrix* lm,
                            uint16_t led_id,
                            uint8_t color,
                            uint8_t ground){
            lm_ = lm;
            led_id_ = led_id;
            color_ = color;
            ground_ = ground;
        }
        blink(){}
        ~blink(){}

        void setBlinkingLed(uint16_t led_id){
            led_id_ = led_id;
        }

        void blinkOn(){
            lm_->save_n_set(color_, led_id_, ground_);
        }
        void blinkOff(){
            lm_->clr_n_restore(led_id_, ground_);
        }

        void startBlinking(uint32_t blinkingTime){
            state_ = STARTED;
            blinkingTime_ = blinkingTime;
            timeCnt_ = 0;
            turnedOnFlag = true;
            blinkOn();
        }
        void stopBlinking(){
            state_ = STOPPED;
            if(turnedOnFlag)
                blinkOff();
        }
        void updateBlinking(){
            if(state_ == STARTED){
                if(timeCnt_ >= blinkingTime_){
                    if(turnedOnFlag){
                        blinkOff();
                    }
                    else {
                        blinkOn();
                    }
                    turnedOnFlag = !turnedOnFlag;
                    timeCnt_ = 0;
                }
            }
        }

};

#endif // __BLINK_H__
