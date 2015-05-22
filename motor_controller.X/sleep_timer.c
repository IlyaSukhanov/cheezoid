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

#include <timer.h>

int timer_flag = 0; // TODO read the register instead of using global
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void){
    timer_flag = 1;
    IFS0bits.T1IF = 0;
}
//Duration is unknown 0xffff is about 6 seconds
void sleep(unsigned int duration){
    timer_flag = 0;
    ConfigIntTimer1(T1_INT_PRIOR_1 & T1_INT_ON);
    WriteTimer1(0);
    OpenTimer1(T1_ON & T1_GATE_OFF & T1_PS_1_256 & T1_SYNC_EXT_OFF & T1_SOURCE_INT, duration);
    while(timer_flag==0);
    CloseTimer1();
}
