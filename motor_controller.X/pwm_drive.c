#include <stdlib.h>
#include "p24Hxxxx.h"
#include "outcompare.h"
#include "sleep_timer.h"
#include "encoder_feedback.h"


//PWM configuration
#define OC_PWM_CONFIG OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE
#define MIN_SPEED 50
unsigned int right_distance = 0;
unsigned int left_distance = 0;
unsigned int right_drive_active = 0;
unsigned int left_drive_active = 0;
unsigned int nominal_speed = 200;

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
    LATAbits.LATA0 = direction;
    LATAbits.LATA1 = direction;
}

// Handle interrupt for PWM timer
void __attribute__ ((__interrupt__, no_auto_psv)) _T2Interrupt(void){
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
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    left_direction(0);
    right_direction(0);

    //Configure timer for PWM
    // TODO use OpenTimer()
    PR2 = 0x00ff;
    IPC1bits.T2IP = 1;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    T2CONbits.TON = 1;
}

void right_speed(int speed){
    right_direction(speed > 0);
    if(abs(speed) < MIN_SPEED)
        speed = 0;
    OpenOC1(OC_PWM_CONFIG, speed, speed);
    OpenOC3(OC_PWM_CONFIG, speed, speed);
}

void left_speed(int speed){
    left_direction(speed > 0);
    if(abs(speed) < MIN_SPEED)
        speed = 0;
    OpenOC2(OC_PWM_CONFIG, speed, speed);
    OpenOC4(OC_PWM_CONFIG, speed, speed);
}

unsigned int is_drive_active(){
    return right_drive_active || left_drive_active;
}

void right_stop(){
    // TODO keep direction opposite of 'down',  I gues thats 'up'
    right_drive_active = 0;
    right_speed(0);
}

void left_stop(){
    // TODO keep direction opposite of 'down', I guess thats 'up'
    left_drive_active = 0;
    left_speed(0);
}

void left_drive(int distance){
    // TODO reset interrupt FIFO?
    // TODO multiply by 2 so we don't need to divide in interrupt handler
    left_distance = abs(distance);
    left_drive_active = left_distance>0;
    if(distance<0){
        left_speed(-nominal_speed);
    }else{
        left_speed(nominal_speed);
    }
}

void right_drive(int distance){
    // TODO reset interrupt FIFO?
    // TODO multiply by 2 so we don't need to divide in interrupt handler
    right_distance = abs(distance);
    right_drive_active = right_distance>0;
    if(distance<0){
        right_speed(-nominal_speed);
    }else{
        right_speed(nominal_speed);
    }
}

unsigned int right_distance_remaining(unsigned int travel){
    if (travel < right_distance){
        right_distance -= travel;
    }else{
        right_distance = 0;
    }
    return right_distance;
}

unsigned int left_distance_remaining(unsigned int travel){
    if (travel < left_distance){
        left_distance -= travel;
    } else {
        left_distance = 0;
    }
    return left_distance;
}

void move(int rotate_distance, int drive_distance){
    if(rotate_distance != 0){
        left_drive(-rotate_distance);
        right_drive(rotate_distance);
        while(is_drive_active());
    }
    if (drive_distance != 0){
        left_drive(drive_distance);
        right_drive(drive_distance);
        while(is_drive_active());
    }
}

void calibration(){
    right_distance = 0xffff;
    left_distance = 0xffff;
    int sleep_time = 0x6000;
    
    /*
    right_speed(255);
    left_speed(255);
    sleep(sleep_time);
    int max_up_right_period =  right_front_period();
    int max_up_left_period = left_front_period();
    right_speed(10);
    left_speed(10);
    
    sleep(sleep_time);

    right_speed(-255);
    left_speed(-255);
    sleep(sleep_time);
    int max_down_right_period =  right_front_period();
    int max_down_left_period = left_front_period();
    right_speed(10);
    left_speed(10);
    */

    int max_stop_speed=0;
    right_speed(max_stop_speed);
    left_speed(max_stop_speed);
    sleep(10000);
    int last_encoder = left_front_encoder_count();
    sleep(20000);
    if (last_encoder != left_front_encoder_count()){
        right_speed(200);
        left_speed(200);
    }
/*
    while(last_encoder != right_front_encoder_count()){
        max_stop_speed++;
        right_speed(max_stop_speed);
        left_speed(max_stop_speed);
        last_encoder = right_front_encoder_count();
        sleep(30000);
    }
 */
    sleep(0xfff);

    right_distance = 0;
    left_distance = 0;


}
