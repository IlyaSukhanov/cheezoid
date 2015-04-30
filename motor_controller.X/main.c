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
 * Pinout cheezoid platform 1
 * pin2     LF motor direction  (channel 2)
 * pin3     LR motor direction  (channel 4)
 * pin4     ????? occupied by microstick debugger
 * pin5     ????? occupied by microstick debugger
 * pin6     RF motor speed  OC1 (channel 1)
 * pin7     RR motor speed  OC3 (channel 3)
 * pin9     RF motor direction  (channel 1)
 * pin10    ???? won't flip bit
 * pin11    RR motor direction  (channel 3)
 * pin14    RP5 FR encoder 5V   IC1 (channel 1)
 * pin15    RP6 FL encoder 5V   IC2 (channel 2)
 * pin16    RP7 RR encoder 5V   IC7 (channel 3)
 * pin17    RP8 RL encoder 5V   IC8 (channel 4)
 * pin18    SPI SCLK yellow     RP9
 * pin21    SPI MOSI red        RP10
 * pin22    SPI MISO orange     RP11
 * pin25    SPI CE0  green      RP14
 * pin23    LF motor speed  OC2 (channel 2)
 * pin24    RR motor speed  OC4 (channel 4)
 *
 * Pinout cheezoid platform 2
 * pin2     white   l motor in1 RA0
 * pin3     black   l motor in2 RA1
 * pin4             occupied by microstick debugger
 * pin5             occupied by microstick debugger
 * pin6     brown   l motor d2  RP2
 * pin7     yellow  r motor d2  RP3
 * pin9     green   r motor in1 RA2
 * pin10            won't flip bit
 * pin11    blue    r motor in2 RB4
 * pin15    white   l enc B     RP6
 * pin16    gray    l enc A     RP7
 * pin17    purple  r enc B     RP8
 * pin18    blue    r enc A     RP9
 * pin19    brown   ground
 * pin21    yellow  SPI SCLK    RP10
 * pin22    red     SPI MOSI    RP11
 * pin23    orange  SPI MISO    RP12
 * pin24    green   SPI CE0     RP13
 *
 */

int main(int argc, char** argv) {
    RCONbits.SWDTEN = 0;

    // Configure pin out
    PPSUnLock;
    // map output compare to our pwm drive pins
    PPSOutput(OUT_FN_PPS_OC1, OUT_PIN_PPS_RP2);
    PPSOutput(OUT_FN_PPS_OC2, OUT_PIN_PPS_RP3);

    //map input compare to out encoder feedback pins
    PPSInput(IN_FN_PPS_IC8, IN_PIN_PPS_RP6);
    PPSInput(IN_FN_PPS_IC2, IN_PIN_PPS_RP7);
    PPSInput(IN_FN_PPS_IC7, IN_PIN_PPS_RP8);
    PPSInput(IN_FN_PPS_IC1, IN_PIN_PPS_RP9);

    //Map SPI pins
    PPSOutput(OUT_FN_PPS_SDO1, OUT_PIN_PPS_RP12);
    PPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP11);
    PPSInput(IN_FN_PPS_SCK1, IN_PIN_PPS_RP10);
    PPSInput(IN_FN_PPS_SS1, IN_PIN_PPS_RP13);
    PPSLock;

    configure_drive();
    configure_encoders();
    configure_command_control();

    //drive_calibration();
    /*
    int i = 0;
    while(i<10){
        move(90,0);
        move(-90,0);
        i++;
    }
    */
    move(0,9999);
    int *move_parameters = 0;
    while(1){
        move_parameters = read_spi_buffer();
        if(move_parameters){
            move(move_parameters[0], move_parameters[1]);
        }
        ClrWdt();
    }
    return 0;
}

