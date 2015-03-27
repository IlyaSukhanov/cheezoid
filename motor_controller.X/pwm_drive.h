/* 
 * File:   pwm_drive.h
 * Author: tox
 *
 * Created on March 18, 2015, 11:30 PM
 */

#ifndef PWM_DRIVE_H
#define	PWM_DRIVE_H

void configure_drive();
void move(int rotate_distance, int drive_distance);
void right_drive_tick(const unsigned int ticks, const int period);
void left_drive_tick(const unsigned int ticks, const int period);
int drive_calibration();

#endif	/* PWM_DRIVE_H */

