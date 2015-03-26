from RPIO import PWM
import time

PEN_UP_PWM_TIME = 600
GPIO_PIN = 17

class CheezoidPenUp():
    def __enter__(self):
        self.pen_servo = PWM.Servo()
        self.pen_servo.set_servo(GPIO_PIN, PEN_UP_PWM_TIME)
        return self

    def __exit__(self, type, value, traceback):
        self.pen_servo.stop_servo(GPIO_PIN)

if __name__ == "__main__":
    with CheezoidPenUp() as pen:
        time.sleep(5)
