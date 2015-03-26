import time
import sys
import signal
import os

RUNNING = True
PIPENAME = "/tmp/cheese_plate"

def int_signal_handler(signal, frame):
    global RUNNING
    RUNNING = False

def create_fifo(pipename):
    if not (os.path.isfile(pipename)):
        os.mkfifo(pipename)
    fifo = open(pipename, 'r')
    return fifo

def repl(fifo):
    print("cheesed repl loop")

def main():
    fifo = create_fifo(PIPENAME)
    signal.signal(signal.SIGINT, int_signal_handler)
    while(RUNNING):
        time.sleep(1)
        repl(fifo)


if __name__ == '__main__':
    main()