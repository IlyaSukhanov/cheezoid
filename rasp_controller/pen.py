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
