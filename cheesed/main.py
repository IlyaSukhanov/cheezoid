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
    print "Reading commands from: %s" % pipename
    fifo = open(pipename, 'r')
    return fifo

def read_commands(fifo):
    lines = fifo.read()  # non-blocking
    for cmd in lines.split("\n"):
        cmd = cmd.strip()
        if cmd:
            yield cmd

def do_cmd(cmd):
    print "Executing: %s" % cmd

def main():
    fifo = create_fifo(PIPENAME)
    signal.signal(signal.SIGINT, int_signal_handler)
    while(RUNNING):
        time.sleep(1)
        for cmd in read_commands(fifo):
            do_cmd(cmd)
    fifo.close()


if __name__ == '__main__':
    main()
