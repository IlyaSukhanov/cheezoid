from RPIO import PWM

PEN_UP_PWM_TIME = 600
GPIO_PIN = 17

class CheezoidPenUp():
    """
    Controls the cheezoid pen. In natural state its always down (drawing).
    To reposition cheezoid without leaving a trail use this context manager
    as follows:

    with CheezoidPenUp() as pen:
        #move here
    """
    def __enter__(self):
        self.pen_servo = PWM.Servo()
        self.pen_servo.set_servo(GPIO_PIN, PEN_UP_PWM_TIME)
        return self

    def __exit__(self, type, value, traceback):
        self.pen_servo.stop_servo(GPIO_PIN)

if __name__ == "__main__":
    """
    Lift pen for 5 seconds.
    """
    import time
    with CheezoidPenUp() as pen:
        time.sleep(5)
