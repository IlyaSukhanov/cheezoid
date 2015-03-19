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