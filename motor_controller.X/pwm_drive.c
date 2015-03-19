#include "p24Hxxxx.h"
#include "pps.h"
#include "outcompare.h"

//PWM configuration
#define OC_PWM_CONFIG OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE

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

// Handle interrupt for PWM timer
void __attribute__ ((__interrupt__)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;
}

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





