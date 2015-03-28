#include <timer.h>

int timer_flag = 0; // TODO read the register instead of using global
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void){
    timer_flag = 1;
    IFS0bits.T1IF = 0;
}
//Duration is unknown 0xffff is about 6 seconds
void sleep(unsigned int duration){
    timer_flag = 0;
    ConfigIntTimer1(T1_INT_PRIOR_1 & T1_INT_ON);
    WriteTimer1(0);
    OpenTimer1(T1_ON & T1_GATE_OFF & T1_PS_1_256 & T1_SYNC_EXT_OFF & T1_SOURCE_INT, duration);
    while(timer_flag==0);
    CloseTimer1();
}
