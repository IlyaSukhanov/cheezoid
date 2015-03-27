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
unsigned int left_encoder_count();
unsigned int right_encoder_count();
unsigned int left_period();
unsigned int right_period();

#endif	/* ENCODER_FEEDBACK_H */

