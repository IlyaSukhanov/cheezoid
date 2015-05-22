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

import os
import sys
import re

PIPENAME = '/tmp/cheese_plate'

def create_fifo(pipename):
    if not os.path.exists(pipename):
        os.mkfifo(pipename, 0600)
    else:
        os.chmod(pipename, 0600)
    fd = os.open(pipename, os.O_RDWR|os.O_NONBLOCK)
    fifo = os.fdopen(fd, 'w')
    return fifo

def cmd_repl(fifo, data):
    fifo = create_fifo(PIPENAME)
    cmds = sanitize_cmd(data)
    for cmd in cmds:
        fifo.write('%s\n' % (cmd))
        fifo.flush()
    return cmds

def sanitize_cmd(input_cmd):
    """
    command has to be
    move (angle, distance)
        angle = -180 to 180 in degrees
        distance = distance in centimeters. floats > 0
    pen [action]
         action = 'up', 'down'
    set
    reset
    """
    results = []
    #print('raw input')
    #print(input_cmd)
    cmds = input_cmd.replace('\r','').replace('\\n', '\n').split('\n')
    print('unsanitized cmds: %s' % ",".join(cmds))
    for cmd in cmds:
        cmd = cmd.strip()
        if re.match('^set\s*$', cmd):
            results.append('set')
        elif re.match('^reset\s*$', cmd):
            results.append('reset')
        elif re.match('^pen\s*(up|down)', cmd):
            match = re.match('^pen\s*(up|down)', cmd)
            results.append('pen %s' % match.groups()[0])
        elif re.match('^move\s*\(\s*([^,\(\)]+)\s*,\s*([^,\(\)]+)\s*\)$', cmd):
            # check value range. reject bad one
            try:
                match = re.match('^move\s*\(\s*([^,\(\)]+)\s*,\s*([^,\(\)]+)\s*\)$', cmd)
                degree = float(match.groups()[0])
                distance = float(match.groups()[1])
                if (degree <= 180 and degree >= -180 and distance >= 0):
                    results.append('move %s %s' % (str(degree), str(distance)))
            except ValueError as err:
                pass
    print("sanitized cmds: %s" % ",".join(results))
    sys.stdout.flush()
    return results
