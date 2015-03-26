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

def do_cmd(cmd):
    print "Executing: %s" % cmd

def main():
    fifo = create_fifo(PIPENAME)
    while(RUNNING):
        line = fifo.readline()  # blocking
        cmd = line.strip()
        if cmd != "":
            do_cmd(cmd)
        else:
            time.sleep(1)
    fifo.close()


if __name__ == '__main__':
    main()
