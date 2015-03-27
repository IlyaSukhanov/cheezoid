import math


DEGREE_IN_RADIAN = 0.0174532925

class Cheezoid(object):

    def __init__(self):
        self._total_cmds = []
        self._total_moves = []
        self._current_coords = (0, 0)

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

        print(all_vectors)
        self._current_coords = (sum([x[0] for x in all_vectors]),
                               sum([x[1] for x in all_vectors]))

        return self._current_coords

    def execute_front_cmd(self, front_cmd):
        self._total_cmds.append(front_cmd)
        if front_cmd._cmd == FrontCommands.MOVE:
            self._total_moves.append(front_cmd)
            # to be added move
            print('cheezoid is now at coord: ')
            print(self.where_am_i())
        elif front_cmd == FrontCommands.PEN:
            self.move_pen(front_cmd._param_1)
        elif front_cmd == FrontCommands.SET:
            cmds = self.get_origin_move()
            for cmd in cmds:
                # to be added, execute reoriented movement
                pass
            self.set_current_position_as_origin()
        elif front_cmd == FrontCommands.RESET:
            cmds = self.get_reset_moves()
            for cmd in cmds:
                # to be added execute moves to move back origin
                pass
        print(self._total_moves)
        print(self._total_cmds)

    def convert_cmd(self):
        pass

    def move_pen(self, direction):
        pass

    def move_motor(self, gyro, angle, distance):
        pass

    def set_current_position_as_origin(self):
        self._total_moves = []
        self._total_cmds = []
        pass

    def get_origin_move(self):
        return

    def get_reset_move(self):
        return

class FrontCommands(object):

    SET = 0
    RESET = 1
    MOVE = 2
    PEN = 3
    UP = 0
    DOWN = 1

    def __init__(self, cmd, param_1=None, param_2=None):
        self._cmd = cmd
        self._param_1 = param_1
        self._param_2 = param_2


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

