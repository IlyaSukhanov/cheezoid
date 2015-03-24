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
#include "command_control.h"

/*
 * Pinout
 * pin4     ????? occupied by microstick debugger
 * pin5     ????? occupied by microstick debugger
 * pin6     RF motor speed  OC1 (channel 1)
 * pin7     RR motor speed  OC3 (channel 3)
 * pin9     RF motor direction  (channel 1)
 * pin10    ???? won't flip bit
 * pin11    RR motor direction  (channel 3)
 * pin14    RF encoder 5V   IC1 (channel 1)
 * pin15    LF encoder 5V   IC2 (channel 2)
 * pin16    RR encoder 5V   IC7 (channel 3)
 * pin17    LR encoder 5V   IC8 (channel 4)
 * pin18    SPI SCLK yellow     RP9
 * pin21    SPI MOSI red        RP10
 * pin22    SPI MISO orange     RP11
 * ?????    SPI CE0  green      NOT USED
 * pin23    LF motor speed  OC2 (channel 2)
 * pin24    RR motor speed  OC4 (channel 4)
 * pin25    LF motor direction  (channel 2)
 * pin26    LR motor direction  (channel 4)
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

    //Map SPI pins
    PPSOutput(OUT_FN_PPS_SDO1, OUT_PIN_PPS_RP11);
    PPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP10);
    PPSInput(IN_FN_PPS_SCK1, IN_PIN_PPS_RP9);
    PPSLock;

    configure_drive();
    configure_encoders();
    configure_command_control();

    // 48 interrupts per wheel rotation
    /*
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
    */
    //move_to(50,0);
    while(1)
    {
        Idle()
    }
    return 0;

}

