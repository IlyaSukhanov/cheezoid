try:
    # likely to fail on non-raspberries
    from RPIO import PWM
except:
    print "WARNING: could not import RPIO"
    Servo = type("servo_mock", (object,), {"set_servo": lambda self, pin, uptime: None, "stop_servo": lambda self, pin: None})()
    PWM = type("rpio_mock", (object,), {"Servo": lambda self: Servo})()

PEN_UP_PWM_TIME = 600
GPIO_PIN = 17


class CheezoidPenControl():
    """
    Controls the cheezoid pen. In natural state its always down (drawing).
    To reposition cheezoid without leaving a trail use this context manager
    as follows:

    with CheezoidPenControl(pen_up=True) as pen:
        #move here
    """
    def __init__(self, pen_up=False):
        self.pen_servo = PWM.Servo()
        self.pen_up = pen_up

    def __enter__(self):
        self.servo_control(self.pen_up)
        return self

    def __exit__(self, type, value, traceback):
        self.servo_control(not self.pen_up)

    def servo_control(self, pen_up):
        if pen_up:
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
