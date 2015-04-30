#include <stdlib.h>
#include "p24Hxxxx.h"
#include "outcompare.h"
#include "sleep_timer.h"
#include "encoder_feedback.h"


//PWM configuration
#define OC_PWM_CONFIG OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE
unsigned int right_distance = 0;
unsigned int left_distance = 0;
int left_speed = 0;
int right_speed = 0;
int max_stop_speed = 0;

#define NOMINAL_SPEED 170
#define SLOW_SPEED 170
#define MIN_SPEED 120
#define SPEED_ADJUST 1

int nominal_period = 0;
int slow_period = 0;

void inline left_direction(unsigned int direction){
    if ( direction > 0 ){
        LATAbits.LATA0 = 0;
        LATAbits.LATA1 = 1;
    }else{
        LATAbits.LATA0 = 1;
        LATAbits.LATA1 = 0;
    }
}

void inline right_direction(unsigned int direction){
    if ( direction > 0 ){
        LATAbits.LATA2 = 1;
        LATBbits.LATB4 = 0;
    }else{
        LATAbits.LATA2 = 0;
        LATBbits.LATB4 = 1;
    }
}

// Handle interrupt for PWM timer
void __attribute__ ((__interrupt__, no_auto_psv)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;
}

void configure_drive(){
    //Configure PWM pins for output
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    //Disable Open Drain
    ODCBbits.ODCB2 = 0x0;
    ODCBbits.ODCB3 = 0x0;

    //Configure direction pins for output
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISBbits.TRISB4 = 0;
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

void set_left_speed(int speed){
    left_direction(speed > 0);
    left_speed = speed;
    speed=abs(speed);
    if(speed < MIN_SPEED){
        speed = 0;
    }
    OpenOC1(OC_PWM_CONFIG, speed, speed);
}

void set_right_speed(int speed){
    right_direction(speed > 0);
    right_speed = speed;
    speed=abs(speed);
    if(speed < MIN_SPEED){
        speed = 0;
    }
    OpenOC2(OC_PWM_CONFIG, speed, speed);
}

int speed_adjust(const int speed, const int our_period, const int their_period){
    if (our_period < their_period){
        //speed down
        if (speed  < 0 ){
            // going down
            return speed + SPEED_ADJUST;
        }else if(speed > 0){
            // going up
            return speed - SPEED_ADJUST;
        }
    } else if(our_period > their_period){
        //speed up
        if (speed < 0 ){
            // going down
            return speed - SPEED_ADJUST;
        }else if(speed > 0){
            // going up
            return speed + SPEED_ADJUST;
        }
    }
    return speed; 
}

unsigned int is_drive_active(){
    return right_distance > 0 && left_distance > 0;
}

void right_stop(){
    right_distance = 0;
    set_right_speed(max_stop_speed);
}

void left_stop(){
    left_distance = 0;
    set_left_speed(max_stop_speed);
}

void left_drive(const int distance, const int speed){
    left_distance = abs(distance);
    if (distance == 0) {
        left_stop();
    } if (distance < 0){
        set_left_speed(-speed);
    }else if(distance > 0 ){
        set_left_speed(speed);
    }
}

void right_drive(const int distance, const int speed){
    right_distance = abs(distance);
    if (distance == 0) {
        right_stop();
    } if (distance < 0){
        set_right_speed(-speed);
    }else if(distance > 0 ){
        set_right_speed(speed);
    }
}

void right_drive_tick(const unsigned int ticks, const int our_period, const int their_period){
    if (ticks < right_distance){
        right_distance -= ticks;
        if(right_distance<30){
            if(right_speed<0){
                set_right_speed(-SLOW_SPEED);
            }else{
                set_right_speed(SLOW_SPEED);
            }
        }else{
            speed_adjust(right_speed, our_period, their_period);
            set_right_speed(right_speed);
        }
    } else {
        right_stop();
    }
}

void left_drive_tick(const unsigned int ticks, const int our_period, const int their_period){
    //right_drive(10, NOMINAL_SPEED);
    if (ticks < left_distance){
        left_distance -= ticks;
        if(left_distance<30){
            if(left_speed<0){
                set_left_speed(-SLOW_SPEED);
            }else{
                set_left_speed(SLOW_SPEED);
            }
        }else{
            set_left_speed(speed_adjust(left_speed, our_period, their_period));
        }
    } else {
        left_stop();
    }
}

void move(const int rotate_distance,const int drive_distance){
    if(rotate_distance != 0){
        if(rotate_distance < 0){
            left_drive(-rotate_distance, NOMINAL_SPEED);
            right_drive(rotate_distance, NOMINAL_SPEED);
        }else{
            left_drive(-rotate_distance, NOMINAL_SPEED);
            right_drive(rotate_distance, NOMINAL_SPEED);
        }
        while(is_drive_active());
    }
    if (drive_distance != 0){
        left_drive(drive_distance, NOMINAL_SPEED);
        right_drive(drive_distance, NOMINAL_SPEED);
        while(is_drive_active());
    }
}

void stop_calibration(){
    //Determine force required to not roll down

    //Set distance high so we don't trigger interrupt based stop
    right_distance = 0xffff;
    left_distance = 0xffff;


    // TODO max_stop_speed should be set to MIN_SPEED

    set_right_speed(max_stop_speed);
    set_left_speed(max_stop_speed);
    sleep(5000);
    int last_encoder = left_encoder_count();
    sleep(5000);
    while(last_encoder != left_encoder_count()){
        max_stop_speed+=20;
        set_right_speed(max_stop_speed);
        set_left_speed(max_stop_speed);
        last_encoder = left_encoder_count();
        sleep(5000);
    }

    sleep(5000);

    right_distance = 0;
    left_distance = 0;

    set_right_speed(max_stop_speed);
    set_left_speed(max_stop_speed);
}

/*
void period_calibration(){
    //Set distance high so we don't trigger interrupt based stop
    right_distance = 0xffff;
    left_distance = 0xffff;
    set_right_speed(NOMINAL_SPEED);
    set_left_speed(NOMINAL_SPEED);
    sleep(5000);

    nominal_period = 

}
*/

void drive_calibration(){
    //stop_calibration();
}
