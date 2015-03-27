#include "p24Hxxxx.h"
#include "incap.h"
#include "pwm_drive.h"
#include "encoder_feedback.h"

//Configure Encoders
#define IC_ENCODER_CONFIG IC_IDLE_CON & IC_TIMER3_SRC &  IC_INT_2CAPTURE & IC_EVERY_EDGE
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
unsigned int interrupt7_period = 0;
unsigned int interrupt8_period = 0;

void configure_encoders(){
    //Configure encoder feedback pins for input
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB8 = 1;

    //Turn on timer
    T3CONbits.TON = 1;
    //OpenTimer3(T3_ON & T3_GATE_OFF & T3_PS_1_8 & T3_SOURCE_INT, 0xFF);
    OpenCapture1(IC_ENCODER_CONFIG);
    //OpenCapture2(IC_ENCODER_CONFIG);
    //OpenCapture7(IC_ENCODER_CONFIG);
    OpenCapture8(IC_ENCODER_CONFIG);

    ConfigIntCapture1(IC_ENCODER_INTERRUPT);
    //ConfigIntCapture2(IC_ENCODER_INTERRUPT);
    //ConfigIntCapture7(IC_ENCODER_INTERRUPT);
    ConfigIntCapture8(IC_ENCODER_INTERRUPT);
}

//FR interrupt
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void){
    ReadCapture1(&interrupt1_times[0]);
    interrupt1_period = interrupt1_times[0] - interrupt1_times[1];
    right_drive_tick(1, interrupt1_period);
    IFS0bits.IC1IF = 0;
}

int right_front_encoder_count(){
    return interrupt1_count;
}

int right_front_period(){
    return interrupt1_period;
}

//FL interrupt
//IC1

//RR
//IC7

//RL read interrupt
void __attribute__((__interrupt__, no_auto_psv)) _IC8Interrupt(void){
    ReadCapture8(&interrupt8_times[0]);
    interrupt8_count++;
    interrupt8_period = interrupt8_times[0] - interrupt8_times[1];
    left_drive_tick(1, interrupt8_period);
    IFS1bits.IC8IF = 0;
}

int left_rear_encoder_count(){
    return interrupt8_count;
}

int left_rear_period(){
    return interrupt8_period;
}