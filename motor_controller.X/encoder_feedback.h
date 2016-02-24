/*
Copyright 2015 Ilya Sukhanov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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

