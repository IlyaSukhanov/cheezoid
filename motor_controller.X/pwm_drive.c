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
unsigned int left_speed = 0;
unsigned int right_speed = 0;
unsigned int desired_period = 0;
int right_desired_direction = 0;
int left_desired_direction = 0;
int max_stop_speed = 0;

//in flat land mode these should be set to the same value
#define ADVANTAGED_SPEED 50 
#define DISADVANTAGED_SPEED 255 
#define ADVANTAGED_SPEED_ADJUST 10 
#define DISADVANTAGED_SPEED_ADJUST 20

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

void set_right_speed(int speed){
    right_direction(speed > 0);
    right_speed = speed;
    speed=abs(speed);
    if(speed < MIN_SPEED){
        speed = 0;
    }
    OpenOC1(OC_PWM_CONFIG, speed, speed);
    OpenOC3(OC_PWM_CONFIG, speed, speed);
}

void set_left_speed(int speed){
    left_direction(speed > 0);
    left_speed = speed;
    speed=abs(speed);
    if(speed < MIN_SPEED){
        speed = 0;
    }
    OpenOC2(OC_PWM_CONFIG, speed, speed);
    OpenOC4(OC_PWM_CONFIG, speed, speed);
}

void right_speed_adjust(const int our_period, const int their_period){
    int target_period;
    int new_speed = right_speed;
    if (right_desired_direction == -1 && left_desired_direction == 1){
        target_period = their_period;
    }else{
        target_period = desired_period; 
    }
    if (our_period < target_period){
        //speed down
        if (right_desired_direction < 0 ){
            //going down
            new_speed += ADVANTAGED_SPEED_ADJUST;
        }else if(right_desired_direction > 0){
            //going up
            new_speed -= DISADVANTAGED_SPEED_ADJUST;
        }
    } else if(our_period > target_period){
        //speed up
        if (right_desired_direction < 0 ){
            //going down
            new_speed -= DISADVANTAGED_SPEED_ADJUST;
        }else if(right_desired_direction > 0){
            //going up
            new_speed += ADVANTAGED_SPEED_ADJUST;
        }
    }else{
        return;
    }
    set_right_speed(new_speed);
}

void left_speed_adjust(const int our_period, const int their_period){
    int target_period;
    int new_speed = left_speed;
    if (left_desired_direction == -1 && right_desired_direction == 1){
        target_period = their_period;
    }else{
        target_period = desired_period; 
    }
    if (our_period < target_period){
        //speed down
        if (left_desired_direction < 0 ){
            //going down
            new_speed += DISADVANTAGED_SPEED_ADJUST;
        }else if(left_desired_direction > 0){
            //going up
            new_speed -= ADVANTAGED_SPEED_ADJUST;
        }
    } else if(our_period > target_period){
        //speed up
        if (left_desired_direction < 0 ){
            //going down
            new_speed -= ADVANTAGED_SPEED_ADJUST;
        }else if(left_desired_direction > 0){
            //going up
            new_speed += DISADVANTAGED_SPEED_ADJUST;
        }
    }else{
        return;
    }
    set_left_speed(new_speed);
}

unsigned int is_drive_active(){
    return right_desired_direction != 0 && left_desired_direction != 0;
}

void right_stop(){
    right_distance = 0;
    right_desired_direction = 0;
    set_right_speed(max_stop_speed);
}

void left_stop(){
    left_distance = 0;
    left_desired_direction = 0;
    set_left_speed(max_stop_speed);
}

void left_drive(const int distance, const int speed){
    left_distance = abs(distance);
    if (distance == 0) {
        left_stop();
    } if (distance < 0){
        left_desired_direction = -1;
        set_left_speed(-speed);
    }else if(distance > 0 ){
        left_desired_direction = 1;
        set_left_speed(speed);
    }
}

void right_drive(const int distance, const int speed){
    right_distance = abs(distance);
    if (distance == 0) {
        right_stop();
    } if (distance < 0){
        right_desired_direction = -1;
        set_right_speed(-speed);
    }else if(distance > 0 ){
        right_desired_direction = 1;
        set_right_speed(speed);
    }
}

void right_drive_tick(const unsigned int ticks, const int our_period, const int their_period){
    if (ticks < right_distance){
        right_distance -= ticks;
        right_speed_adjust(our_period, their_period);
    } else {
        right_stop();
    }
}

void left_drive_tick(const unsigned int ticks, const int our_period, const int their_period){
    if (ticks < left_distance){
        left_distance -= ticks;
        left_speed_adjust(our_period, their_period);
    } else {
        left_stop();
    }
}

void move(const int rotate_distance,const int drive_distance){
    if(rotate_distance != 0){
        if(rotate_distance < 0){
            left_drive(-rotate_distance, DISADVANTAGED_SPEED);
            right_drive(rotate_distance, ADVANTAGED_SPEED);
        }else{
            left_drive(-rotate_distance, ADVANTAGED_SPEED);
            right_drive(rotate_distance, DISADVANTAGED_SPEED);
        }
        while(is_drive_active());
    }
    if (drive_distance != 0){
        left_drive(drive_distance, ADVANTAGED_SPEED);
        right_drive(drive_distance, ADVANTAGED_SPEED);
        while(is_drive_active());
    }
}

void wheel_period_calibration(){
    right_distance = 0xffff;
    left_distance = 0xffff;
    
    set_right_speed(255);
    set_left_speed(255);
    sleep(10000);
    int max_up_right_period =  right_period();
    int max_up_left_period = left_period();

    desired_period = (max_up_right_period + max_up_left_period);
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

}

void drive_calibration(){
    stop_calibration();
    wheel_period_calibration();
    set_right_speed(max_stop_speed);
    set_left_speed(max_stop_speed);
}

/*
void turnClockwise(int stop_speed) {
    right_distance = 0xffff;
    left_distance = 0xffff;
    set_left_speed(255);
    set_right_speed(stop_speed);

    sleep(20000);
    set_left_speed(stop_speed-10);
    set_right_speed(stop_speed-10);

    sleep(0xfff);
    right_distance = 0;
    left_distance = 0;
}
*/
