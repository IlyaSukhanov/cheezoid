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

#ifndef PWM_DRIVE_H
#define	PWM_DRIVE_H

void configure_drive();
void move(int rotate_distance, int drive_distance);
void right_drive_tick(const unsigned int ticks, const int our_period, const int their_period);
void left_drive_tick(const unsigned int ticks, const int our_period, const int their_period);
int drive_calibration();
void right_stop();
void left_stop();

#endif	/* PWM_DRIVE_H */

