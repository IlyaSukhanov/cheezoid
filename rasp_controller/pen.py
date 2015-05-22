"""
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
"""

try:
    # likely to fail on non-raspberries
    from RPIO import PWM
except:
    print "WARNING: could not import RPIO"
    Servo = type("servo_mock", (object,), {"set_servo": lambda self, pin, uptime: None, "stop_servo": lambda self, pin: None})()
    PWM = type("rpio_mock", (object,), {"Servo": lambda self: Servo})()

PEN_DOWN_HARD_TIME = 2000
PEN_UP_PWM_TIME = 600
GPIO_PIN = 17
PEN_DOWN_HARD = -1
PEN_DOWN = 0
PEN_UP = 1


class CheezoidPenControl():
    """
    Controls the cheezoid pen. In natural state its always down (drawing).
    To reposition cheezoid without leaving a trail use this context manager
    as follows:

    with CheezoidPenControl(pen_up=True) as pen:
        #move here
    """
    def __init__(self, mode=PEN_UP):
        self.pen_servo = PWM.Servo()
        self.pen_mode = mode

    def __enter__(self):
        self.servo_control()
        return self

    def __exit__(self, type, value, traceback):
        self.pen_mode = PEN_DOWN
        self.servo_control()

    def servo_control(self):
        if self.pen_mode == PEN_DOWN_HARD:
            self.pen_servo.set_servo(GPIO_PIN, PEN_DOWN_HARD_TIME)
        elif self.pen_mode == PEN_UP:
            self.pen_servo.set_servo(GPIO_PIN, PEN_UP_PWM_TIME)
        else:
            self.pen_servo.stop_servo(GPIO_PIN)

if __name__ == "__main__":
    """
    Lift pen for 5 seconds.
    """
    import time
    with CheezoidPenControl(pen_up=True) as pen:
        time.sleep(5)
