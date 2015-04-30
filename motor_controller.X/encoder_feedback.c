#include "p24Hxxxx.h"
#include "incap.h"
#include "pwm_drive.h"
#include "encoder_feedback.h"

//Configure Encoders
#define IC_ENCODER_CONFIG IC_IDLE_CON & IC_TIMER3_SRC &  IC_INT_2CAPTURE & IC_EVERY_RISE_EDGE
#define IC_ENCODER_INTERRUPT IC_INT_PRIOR_2 & IC_INT_ON
#define ENCODER_BUFFER_LENGTH 2

unsigned int interrupt1_times[ENCODER_BUFFER_LENGTH];
unsigned int interrupt2_times[ENCODER_BUFFER_LENGTH];
unsigned int interrupt7_times[ENCODER_BUFFER_LENGTH];
unsigned int interrupt8_times[ENCODER_BUFFER_LENGTH];
unsigned int interrupt1_count = 0;
unsigned int interrupt2_count = 0;
unsigned int interrupt7_count = 0;
unsigned int interrupt8_count = 0;
unsigned int interrupt1_period = 0;
unsigned int interrupt2_period = 0;
unsigned int interrupt3_period = 0;
unsigned int interrupt4_period = 0;

void configure_encoders(){
    //Configure encoder feedback pins for input
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB8 = 1;
    TRISBbits.TRISB9 = 1;

    //Turn on timer
    T3CONbits.TON = 1;
    //OpenTimer3(T3_ON & T3_GATE_OFF & T3_PS_1_8 & T3_SOURCE_INT, 0xFF);
    OpenCapture1(IC_ENCODER_CONFIG);
    OpenCapture2(IC_ENCODER_CONFIG);
    //OpenCapture7(IC_ENCODER_CONFIG);
    //OpenCapture8(IC_ENCODER_CONFIG);

    ConfigIntCapture1(IC_ENCODER_INTERRUPT);
    ConfigIntCapture2(IC_ENCODER_INTERRUPT);
    //ConfigIntCapture7(IC_ENCODER_INTERRUPT);
    //ConfigIntCapture8(IC_ENCODER_INTERRUPT);
}

//Right A interrupt
//IC1
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void){
    ReadCapture1(&interrupt1_times[0]);
    interrupt1_count++;
    interrupt1_period = interrupt1_times[1] - interrupt1_times[0];
    right_drive_tick(1, interrupt1_period, interrupt2_period);
    IFS0bits.IC1IF = 0;
}

unsigned int right_encoder_count(){
    return interrupt1_count;
}

unsigned int right_period(){
    return interrupt1_period;
}

//Left A interrupt
//IC2
void __attribute__((__interrupt__, no_auto_psv)) _IC2Interrupt(void){
    ReadCapture2(&interrupt2_times[0]);
    interrupt2_count++;
    interrupt2_period = interrupt2_times[1] - interrupt2_times[0];
    left_drive_tick(1, interrupt2_period, interrupt1_period);
    IFS0bits.IC2IF = 0;
}

unsigned int left_encoder_count(){
    return interrupt2_count;
}

unsigned int left_period(){
    return interrupt2_period;
}

