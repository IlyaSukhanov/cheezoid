"""
Copyright 2015 Ilya Sukhanov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import time
import datetime
import os
import sys
from cheezoid import Cheezoid, FrontCommands

RUNNING = True
PIPENAME = "/tmp/cheese_plate"

def int_signal_handler(signal, frame):
    global RUNNING
    RUNNING = False

def create_fifo(pipename):
    if not os.path.exists(pipename):
        os.mkfifo(pipename, 0600)
    else:
        os.chmod(pipename, 0600)
    print "Reading commands from: %s" % pipename
    sys.stdout.flush()
    fd = os.open(pipename, os.O_RDWR)
    fifo = os.fdopen(fd, 'r')
    return fifo

def do_cmd(cheezoid, cmd):
    print(datetime.datetime.now())
    print("Executing: %s" % cmd)
    front_cmd = FrontCommands.cmd_factory(cmd)
    cheezoid.execute_front_cmd(front_cmd)
    sys.stdout.flush()

def main():
    cheezoid = Cheezoid()
    fifo = create_fifo(PIPENAME)
    while(RUNNING):
        line = fifo.readline()  # blocking
        cmd = line.strip()
        if cmd != "":
            do_cmd(cheezoid, cmd)
        else:
            time.sleep(1)
    fifo.close()


if __name__ == '__main__':
    main()
