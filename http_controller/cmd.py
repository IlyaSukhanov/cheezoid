import os
import re

PIPENAME = '/tmp/cheese_plate'

def create_fifo(pipename):
    if not os.path.exists(pipename):
        os.mkfifo(pipename)
    fd = os.open(pipename, os.O_RDWR|os.O_NONBLOCK)
    fifo = os.fdopen(fd, 'w')
    return fifo

def cmd_repl(fifo, data):
    fifo = create_fifo(PIPENAME)
    data = sanitize_cmd(data)
    if data != "":
        fifo.write('%s\n' % (data))
        fifo.flush()

def sanitize_cmd(cmd):
    """
    command has to be
    move (angle, distance)
        angle = -180 to 180 in degrees
        distance = distance in centimeters. floats > 0
    pen [action]
         action = 'up', 'down'

    reset
    """
    cmd = cmd.strip()
    if cmd[0:5] == 'reset':
        return cmd[0:5]
    elif cmd[0:3] == 'pen':
        if cmd[3:].strip() in ['up', 'down']:
            return cmd[0:3] + ' ' + cmd[3:].strip()
        else:
            return ""
    elif cmd[0:4] == 'move':
        params = cmd[4:].strip()
        try:
            if params[0] == '(' and params[-1] == ')' and params.index(",") >= 0:
                (degree_str, distance_str) = params[1:-1].split(",")
                distance = float(distance_str)
                degree = float(degree_str)
                # degree has to be -180.0 to 180
                if degree > 180 or degree < -180:
                    return ""
                # distance has to  > 0
                elif distance <= 0:
                    return ""
                else:
                    return "move (%s, %s)" %(str(degree), str(distance))
        except ValueError as err:
            return ""
        else:
            return ""
    else:
        return ""
