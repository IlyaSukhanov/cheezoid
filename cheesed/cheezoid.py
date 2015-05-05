import math
from rasp_controller.drive import CheezoidDrive
from rasp_controller.pen import (
    CheezoidPenControl,
    PEN_DOWN,
    PEN_DOWN_HARD,
    PEN_UP,
)
import time

WHEEL_CIRCUMFERENCE = 4.2 * 3.1415
TICKS_PER_ROTATION = 447.0
TICKS_PER_CM = TICKS_PER_ROTATION / WHEEL_CIRCUMFERENCE
TICKS_PER_DEGREE = 1057.0 / 360.0
DEGREE_IN_RADIAN = 0.0174532925
MAX_SUPPORTED_DEGREE = 90
TOTAL_DEGREES = 360

class Cheezoid(object):

    def __init__(self):
        self._total_cmds = []
        self._total_moves = []
        self._current_coords = (0, 0)
        self._pen_state = FrontCommands.DOWN
        self._cheezoid_drive = CheezoidDrive()
        self._alignment = True

    def current_turtle_angle(self):
        all_angles = [m._param_1 for m in self._total_moves]
        accumulated_angle = []
        sum_angle = 0
        for angle in all_angles:
            sum_angle = sum_angle + angle
            if sum_angle > 0:
                sum_angle = sum_angle % TOTAL_DEGREES
            elif sum_angle < 0:
                sum_angle = -1.0 * (math.fabs(sum_angle) % TOTAL_DEGREES)
        return sum_angle

    def where_am_i(self):
        # compute current coord,
        # algorithm:
        # we have total_moves, so we know initial position at origin (0, 0), with cheezoid face 0 relative degree.
        # then for each step, the command is a location relative angle to current orientation.
        # so the final position is the sum of vectors for each move
        # for each move. the vector is compute as the
        # (x, y) = distance * (cos(total accumulated angle changes), sin(total accumulated angle changes)
        # the current location is
        # sum of (all move vectors)
        # this model for the moment ignore errors and other servo adjustment for hold itself to the white board.
        # so we will see how to integrate with feedbacks later

        all_distances = [m._param_2 for m in self._total_moves]
        all_angles = [m._param_1 for m in self._total_moves]
        accumulated_angle = []
        sum_angle = 0
        for angle in all_angles:
            sum_angle = sum_angle + angle
            accumulated_angle.append(sum_angle)
        all_vectors = []
        angle_distance_pairs = zip(accumulated_angle, all_distances)
        for angle_distance in angle_distance_pairs:
            angle_in_radian = angle_distance[0] * DEGREE_IN_RADIAN
            all_vectors.append((angle_distance[1] * math.cos(angle_in_radian),
                               angle_distance[1] * math.sin(angle_in_radian)))

        #print(all_vectors)
        self._current_coords = (sum([x[0] for x in all_vectors]),
                               sum([x[1] for x in all_vectors]))

        return self._current_coords

    def execute_front_cmd(self, front_cmd):
        self._total_cmds.append(front_cmd)
        if front_cmd._cmd == FrontCommands.MOVE:
            self._total_moves.append(front_cmd)
            self.move(front_cmd)
        elif front_cmd._cmd == FrontCommands.PEN:
            self.move_pen(front_cmd._param_1)
        elif front_cmd._cmd == FrontCommands.SET:
            self.set_current_position_as_origin()
        elif front_cmd._cmd == FrontCommands.RESET:
            self.get_reset_moves()

    def move(self, move_cmd):
        print('sending move command(s): %s with pen %s' % (move_cmd, self._pen_state))
        (angle_degrees, distance_cm) = move_cmd.params

        print("in degrees:{0}".format(angle_degrees))
        if abs(angle_degrees) >= MAX_SUPPORTED_DEGREE:
            angle_degrees = (abs(angle_degrees)/angle_degrees) * (abs(angle_degrees) - 180)
            self._alignment = not self._alignment
        if not self._alignment:
            distance_cm = -1 * distance_cm
        print("out degrees:{0}".format(angle_degrees))

        angle_ticks = int(-angle_degrees * TICKS_PER_DEGREE)
        print("distance: {0}*{1}=".format(distance_cm, TICKS_PER_CM, int(distance_cm * TICKS_PER_CM)))
        distance_ticks = int(distance_cm * TICKS_PER_CM)
        pen_state = PEN_UP if (self._pen_state == FrontCommands.UP) else PEN_DOWN

        #with CheezoidPenControl(mode=pen_state):
        self._cheezoid_drive.move(angle_ticks, distance_ticks)
        print('cheezoid is now at coord: %s' % ",".join(map(str, self.where_am_i())))

    def move_pen(self, direction):
        print("pen state: %s" % ("down" if direction == FrontCommands.DOWN else "up"))
        self._pen_state = direction

    def move_motor(self, gyro, angle, distance):
        pass

    def set_current_position_as_origin(self):
        self._total_moves = []
        self._total_cmds = []
        self._alignment = True
        self._pen_state = FrontCommands.DOWN
        self._current_coords = (0.0,0.0)
        pass

    def get_origin_move(self):
        return []

    def get_reset_moves(self):
        current_total_angle = self.current_turtle_angle()
        current_location = self.where_am_i()
        distance = math.sqrt(current_location[0] * current_location[0] +
                             current_location[1] * current_location[1])
        vec = (current_location[0]/distance, current_location[1]/distance)
        position_angle = math.acos(vec[0]) / DEGREE_IN_RADIAN
        if vec[1] > 0:
            position_angle = -1.0 * position_angle

        if self._alignment:
            distance = -1 * distance

        angle_diff = position_angle - current_total_angle
        if angle_diff > 180:
            angle_diff = angle_diff - 360
        elif angle_diff < -180:
            angle_diff = angle_diff + 360
        self.move(FrontCommands(cmd=FrontCommands.MOVE, param_1=angle_diff, param_2=distance))
        # need to block for next command in this case
        time.sleep(20)
        if position_angle >= 0:
            if self._alignment:
                self.move(FrontCommands(cmd=FrontCommands.MOVE, param_1=180 - position_angle, param_2=0))
            else:
                self.move(FrontCommands(cmd=FrontCommands.MOVE, param_1=-1.0 * position_angle, param_2=0))
        else:
            if self._alignment:
                self.move(FrontCommands(cmd=FrontCommands.MOVE, param_1=-1.0*(180 + position_angle), param_2=0))
            else:
                self.move(FrontCommands(cmd=FrontCommands.MOVE, param_1=-1.0 * position_angle, param_2=0))
        self.set_current_position_as_origin()
        return

class FrontCommands(object):

    SET = 0
    RESET = 1
    MOVE = 2
    PEN = 3
    UP = 0
    DOWN = 1

    COMMAD_STR = {
        SET : 'set',
        RESET: 'reset',
        MOVE: 'move',
        PEN: 'pen'
    }

    def __init__(self, cmd, param_1=None, param_2=None):
        self._cmd = cmd
        self._param_1 = param_1
        self._param_2 = param_2

    def __repr__(self):
        if self._param_2:
            return "%s %s %s" % (self.COMMAD_STR[self._cmd], self._param_1, self._param_2)
        elif self._param_1:
            return "%s %s" % (self.COMMAD_STR[self._cmd], self._param_1)
        elif self._cmd:
            return "%s" % (self.COMMAD_STR[self._cmd])

    @classmethod
    def cmd_factory(cls, cmd):
        if cmd == 'set':
            return FrontCommands(FrontCommands.SET)
        if cmd == 'reset':
            return FrontCommands(FrontCommands.RESET)
        if cmd[0:3] == 'pen':
            a = cmd.split(' ')
            direction = FrontCommands.UP if a[1] == 'up' else FrontCommands.DOWN
            return FrontCommands(FrontCommands.PEN, direction)
        if cmd[0:4] == 'move':
            a = cmd.split(' ')
            return FrontCommands(FrontCommands.MOVE, float(a[1]), float(a[2]))

    @property
    def params(self):
        return (self._param_1, self._param_2)
