#include "p24Hxxxx.h"
#include "incap.h"
#include "pwm_drive.h"
#include "timer.h"
#include "encoder_feedback.h"

//Configure Encoders
#define IC_ENCODER_CONFIG IC_IDLE_CON & IC_TIMER3_SRC & IC_INTERRUPT

unsigned int encoder1_times[2];

void configure_encoders(){
    //Configure encoder feedback pins for input
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB8 = 1;

    //Turn on timer
    T3CONbits.TON = 1;
    //OpenTimer3(T3_ON & T3_GATE_OFF & T3_PS_1_8 & T3_SOURCE_INT, 0xFF);
    OpenCapture1(IC_IDLE_CON & IC_TIMER3_SRC &  IC_INT_2CAPTURE & IC_EVERY_EDGE);

    ConfigIntCapture1(IC_INT_PRIOR_1 & IC_INT_ON);


}

void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void){
    ReadCapture1(&encoder1_times[0]);
    //if (encoder1_times[0] - encoder1_times[0] > 10){
    //    right_drive(0,0);
    //}else{
        right_drive(100,0);
    //}
    IFS0bits.IC1IF = 0;
}