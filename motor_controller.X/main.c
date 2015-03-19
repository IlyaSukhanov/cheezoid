/* 
 * File:   pwm.c
 *
 * Created on March 3, 2015, 6:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24Hxxxx.h"
#include "pwm_drive.h"
#include "encoder_feedback.h"
#include "sleep_timer.h"
#include "pps.h"

/*
 * Pinout
 * pin4     ????? occupied by microstick debugger
 * pin5     ????? occupied by microstick debugger
 * pin6     right front motor speed     (channel 1)
 * pin7     right rear motor speed      (channel 3)
 * pin9     right front motor direction (channel 1)
 * pin10    ???? won't flip bit
 * pin14    right front encoder 5V      (channel 1)
 * pin15    right front encoder 5V      (channel 2)
 * pin16    right front encoder 5V      (channel 3)
 * pin17    right front encoder 5V      (channel 4)
 * pin11    right rear motor direction  (channel 3)
 * pin23    left front motor speed      (channel 2)
 * pin24    left rear motor speed       (channel 4)
 * pin25    left front motor direction  (channel 2)
 * pin26    left read motor direction   (channel 4)
 */

int main(int argc, char** argv) {
    //RCONbits.SWDTEN = 0;

    // Configure pin out
    PPSUnLock;
    // map output compare to our pwm pins
    PPSOutput(OUT_FN_PPS_OC1, OUT_PIN_PPS_RP2);
    PPSOutput(OUT_FN_PPS_OC2, OUT_PIN_PPS_RP3);
    PPSOutput(OUT_FN_PPS_OC3, OUT_PIN_PPS_RP12);
    PPSOutput(OUT_FN_PPS_OC4, OUT_PIN_PPS_RP13);

    //map input compare to out encoder feedback pins
    PPSInput(IN_FN_PPS_IC1, IN_PIN_PPS_RP5);
    PPSLock;

    configure_drive();
    configure_encoders();
    //left_drive(100, 0);
    //sleep(0xffff);
    //right_drive(100, 0);
    while(1)
    {
        Idle()
    }
    return 0;

}

