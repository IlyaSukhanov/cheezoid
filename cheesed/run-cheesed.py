import time
import datetime
import os
import sys


RUNNING = True
PIPENAME = "/tmp/cheese_plate"

def int_signal_handler(signal, frame):
    global RUNNING
    RUNNING = False

def create_fifo(pipename):
    if not os.path.exists(pipename):
        os.mkfifo(pipename, 0666)
    else:
        os.chmod(pipename, 0666)
    print "Reading commands from: %s" % pipename
    sys.stdout.flush()
    fd = os.open(pipename, os.O_RDWR)
    fifo = os.fdopen(fd, 'r')
    return fifo

def do_cmd(cmd):
    print(datetime.datetime.now())
    print("Executing: %s" % cmd)
    sys.stdout.flush()

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
