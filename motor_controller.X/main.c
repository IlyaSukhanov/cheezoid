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
 * pin6     RF motor speed  OC1 (channel 1)
 * pin7     RR motor speed  OC3 (channel 3)
 * pin9     RF motor direction  (channel 1)
 * pin10    ???? won't flip bit
 * pin14    RF encoder 5V   IC1 (channel 1)
 * pin15    LF encoder 5V   IC2 (channel 2)
 * pin16    RR encoder 5V   IC7 (channel 3)
 * pin17    LR encoder 5V   IC8 (channel 4)
 * pin11    RR motor direction  (channel 3)
 * pin23    LF motor speed  OC2 (channel 2)
 * pin24    RR motor speed  OC4 (channel 4)
 * pin25    LF motor direction  (channel 2)
 * pin26    LR motor direction  (channel 4)
 * ?????    MOSI SPI red
 * ?????    MISO SPI orange
 * ?????    SCLK SPI yellow
 * ?????    CEO  SPI gray
 */

int main(int argc, char** argv) {
    RCONbits.SWDTEN = 0;

    // Configure pin out
    PPSUnLock;
    // map output compare to our pwm pins
    PPSOutput(OUT_FN_PPS_OC1, OUT_PIN_PPS_RP2);
    PPSOutput(OUT_FN_PPS_OC3, OUT_PIN_PPS_RP3);
    PPSOutput(OUT_FN_PPS_OC2, OUT_PIN_PPS_RP12);
    PPSOutput(OUT_FN_PPS_OC4, OUT_PIN_PPS_RP13);

    //map input compare to out encoder feedback pins
    PPSInput(IN_FN_PPS_IC1, IN_PIN_PPS_RP5);
    PPSInput(IN_FN_PPS_IC2, IN_PIN_PPS_RP6);
    PPSInput(IN_FN_PPS_IC7, IN_PIN_PPS_RP7);
    PPSInput(IN_FN_PPS_IC8, IN_PIN_PPS_RP8);
    PPSLock;

    configure_drive();
    configure_encoders();
    // 48 interrupts per wheel rotation
    //left_speed(250, 0);
    //sleep(0xffff);
    //right_speed(250, 0);
    int i = 0;
    while(i<10){
        left_drive(48,0);
        right_drive(48, 0);
        while(is_drive_active()){
            Idle();
        }
        left_drive(48,1);
        right_drive(48,1);
        while(is_drive_active()){
            Idle();
        }
        i++;
    }
    while(1)
    {
        Idle()
    }
    return 0;

}

