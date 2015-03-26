import time
import sys
import signal
import os
import stat

RUNNING = True
PIPENAME = "/tmp/cheese_plate"

def int_signal_handler(signal, frame):
    global RUNNING
    RUNNING = False

def create_fifo(pipename):
    if not os.path.exists(pipename):
        os.mkfifo(pipename)
    fifo = open(pipename, 'r')
    return fifo

def repl(fifo):
    line = fifo.read()  # non-blocking
    sys.stdout.write(line or '.')
    sys.stdout.flush()

def main():
    fifo = create_fifo(PIPENAME)
    signal.signal(signal.SIGINT, int_signal_handler)
    while(RUNNING):
        time.sleep(1)
        repl(fifo)
    fifo.close()


if __name__ == '__main__':
    main()
