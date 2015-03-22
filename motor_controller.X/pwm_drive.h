/* 
 * File:   pwm_drive.h
 * Author: tox
 *
 * Created on March 18, 2015, 11:30 PM
 */

#ifndef PWM_DRIVE_H
#define	PWM_DRIVE_H

void inline left_direction(unsigned int direction);
void inline right_direction(unsigned int direction);
void inline left_drive(unsigned int distance, unsigned int direction);
void inline right_drive(unsigned int distance, unsigned int direction);
void inline left_speed(unsigned int speed, unsigned int direction);
void inline right_speed(unsigned int speed, unsigned int direction);
void configure_drive();
unsigned int right_distance_remaining(unsigned int travel);
unsigned int left_distance_remaining(unsigned int travel);
unsigned int is_drive_active();
void right_stop();
void right_stop();


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* PWM_DRIVE_H */

