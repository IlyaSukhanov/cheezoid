/* 
 * File:   pwm.c
 *
 * Created on March 3, 2015, 6:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24Hxxxx.h"
#include "pps.h"
#include "outcompare.h"
#include <timer.h>
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

void inline right_direction(unsigned int direction){
    if ( direction > 0 ){
        direction = 1;
    }
        LATAbits.LATA2 = direction;
        LATBbits.LATB4 = direction;
}

void inline left_direction(unsigned int direction){
    if ( direction > 0 ){
        direction = 1;
    }
    LATBbits.LATB14 = direction;
    LATBbits.LATB15 = direction;
}

//PWM configuration
// Handle interrupt for PWM timer
void __attribute__ ((__interrupt__)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;
}
#define OC_PWM_CONFIG OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE
void configure_drive(){
    //Configure PWM pins for output
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;
    //Disable Open Drain
    ODCBbits.ODCB2 = 0x0;
    ODCBbits.ODCB3 = 0x0;
    ODCBbits.ODCB12 = 0x0;
    ODCBbits.ODCB13 = 0x0;

    //Configure direction pins for output
    TRISAbits.TRISA2 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB15 = 0;
    right_direction(0);
    left_direction(0);

    // Configure pin out
    PPSUnLock;
    // map output compare to our pwm pins
    PPSOutput(OUT_FN_PPS_OC1, OUT_PIN_PPS_RP2);
    PPSOutput(OUT_FN_PPS_OC2, OUT_PIN_PPS_RP3);
    PPSOutput(OUT_FN_PPS_OC3, OUT_PIN_PPS_RP12);
    PPSOutput(OUT_FN_PPS_OC4, OUT_PIN_PPS_RP13);
    PPSLock;

    //Configure timer for PWM
    PR2 = 0x00ff;
    IPC1bits.T2IP = 1;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    T2CONbits.TON = 1;
}


void left_drive(unsigned int speed, unsigned int direction){
    left_direction(direction);
    OpenOC1(OC_PWM_CONFIG, speed, speed);
    OpenOC2(OC_PWM_CONFIG, speed, speed);
}

void right_drive(unsigned int speed, unsigned int direction){
    right_direction(direction);
    OpenOC3(OC_PWM_CONFIG, speed, speed);
    OpenOC4(OC_PWM_CONFIG, speed, speed);
}

/*
//Configure Encoders
#define IC_ENCODER_CONFIG IC_IDLE_CON & IC_TIMER3_SRC & IC_INTERRUPT
void configure_encoders(){
    //Configure encoder feedback pins for input
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB8 = 1;

    // Configure pin out
    PPSUnLock;
    // map input capture for encoder feedback
    PPSInput(IN_FN_PPS_IC1, IN_PIN_PPS_RP5);
    PPSInput(IN_FN_PPS_IC2, IN_PIN_PPS_RP6);
    PPSInput(IN_FN_PPS_IC7, IN_PIN_PPS_RP7);
    PPSInput(IN_FN_PPS_IC8, IN_PIN_PPS_RP8);
    PPSLock;
}
*/


int timer_flag = 0; // TODO read the register instead of using global
void __attribute__((__interrupt__)) _T1Interrupt(void){
    timer_flag = 1;
    IFS0bits.T1IF = 0;
}
//Duration is unknown 0xffff is about 6 seconds
void timer_sleep(unsigned int duration){
    ConfigIntTimer1(T1_INT_PRIOR_1 & T1_INT_ON);
    WriteTimer1(0);
    OpenTimer1(T1_ON & T1_GATE_OFF & T1_PS_1_256 & T1_SYNC_EXT_OFF & T1_SOURCE_INT, duration);
    while(timer_flag==0);
    CloseTimer1();
}

int main(int argc, char** argv) {
    RCONbits.SWDTEN = 0;

    configure_drive();
    left_drive(200, 1);
    right_drive(200, 0);

    while(1)
    {
        Idle()
    }
    return 0;
}

