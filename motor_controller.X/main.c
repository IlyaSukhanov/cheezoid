/* 
 * File:   pwm.c
 *
 * Created on March 3, 2015, 6:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24Hxxxx.h"
#include "pwm_drive.h"
//#include <InCap.h>

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

    configure_drive();
    left_drive(200, 1);
    right_drive(200, 0);

    while(1)
    {
        Idle()
    }
    return 0;

}

