import stat
import os

PIPENAME = '/tmp/cheese_plate'

def create_fifo(pipename):
    if not os.path.exists(pipename):
        os.mkfifo(pipename)
    fd = os.open(pipename, os.O_RDWR|os.O_NONBLOCK)
    fifo = os.fdopen(fd, 'w')
    return fifo

def cmd_repl(fifo):
    fifo = create_fifo(PIPENAME)
    fifo.write('resett')
    print('in cmd module')