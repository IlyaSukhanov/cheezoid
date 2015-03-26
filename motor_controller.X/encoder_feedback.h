/* 
 * File:   encoder_feedback.h
 * Author: tox
 *
 * Created on March 19, 2015, 12:46 AM
 */

#ifndef ENCODER_FEEDBACK_H
#define	ENCODER_FEEDBACK_H

void configure_encoders();
unsigned int enc_flag_check();
int right_front_period();
int left_front_period();
int right_front_encoder_count();
int left_front_encoder_count();

#endif	/* ENCODER_FEEDBACK_H */

