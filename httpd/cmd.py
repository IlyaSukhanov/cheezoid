import os
import re

PIPENAME = '/tmp/cheese_plate'

def create_fifo(pipename):
    if not os.path.exists(pipename):
        os.mkfifo(pipename, 0666)
    else:
        os.chmod(pipename, 0666)
    fd = os.open(pipename, os.O_RDWR|os.O_NONBLOCK)
    fifo = os.fdopen(fd, 'w')
    return fifo

def cmd_repl(fifo, data):
    fifo = create_fifo(PIPENAME)
    cmds = sanitize_cmd(data)
    for cmd in cmds:
        fifo.write('%s\n' % (cmd))
        fifo.flush()

def sanitize_cmd(input_cmd):
    """
    command has to be
    move (angle, distance)
        angle = -180 to 180 in degrees
        distance = distance in centimeters. floats > 0
    pen [action]
         action = 'up', 'down'

    reset
    """
    results = []
    print('raw input')
    print(input_cmd)
    print('unsanitized cmds')
    cmds = input_cmd.split('\\n')
    print(cmds)
    for cmd in cmds:
        cmd = cmd.strip()
        if cmd[0:5] == 'reset':
            results.append(cmd[0:5])
        elif cmd[0:3] == 'pen':
            if cmd[3:].strip() in ['up', 'down']:
                results.append(cmd[0:3] + ' ' + cmd[3:].strip())
        elif cmd[0:4] == 'move':
            params = cmd[4:].strip()
            try:
                if params[0] == '(' and params[-1] == ')' and params.index(",") >= 0:
                    (degree_str, distance_str) = params[1:-1].split(",")
                    distance = float(distance_str)
                    degree = float(degree_str)
                    # degree has to be -180.0 to 180
                    if degree > 180 or degree < -180:
                        pass
                    # distance has to  > 0
                    elif distance <= 0:
                        pass
                    else:
                        results.append("move (%s, %s)" %(str(degree), str(distance)))
            except ValueError as err:
                pass
            else:
                pass
        else:
            pass
    print("sanitized cmds")
    print(results)
    return results
