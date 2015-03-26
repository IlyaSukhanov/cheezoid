import time
import sys
import signal

RUNNING = True

def int_signal_handler(signal, frame):
    global RUNNING
    RUNNING = False


def main():
    signal.signal(signal.SIGINT, int_signal_handler)
    while(RUNNING):
        time.sleep(1)
        print('cheesed is running')


if __name__ == '__main__':
    main()