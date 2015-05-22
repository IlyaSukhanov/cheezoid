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

import re
import math

def get_lines(svg):
    lines = []
    svg_line_parts = svg.split('<line ')
    for svg_line_part in svg_line_parts:
        parts = svg_line_part.split('/>')
        if len(parts) > 0:
            lines.append(parts[0])
    return lines

def get_line_coords(svg_lines):
    coords = []
    for svg_line in svg_lines:
        (x1, x2, y1, y2) = (None, None, None, None)
        x1_match = re.search('x1=[\'"]{0,1}(\d+)[\'"]{0,1}', svg_line)
        if x1_match:
            x1 = int(x1_match.groups()[0])
        y1_match = re.search('y1=[\'"]{0,1}(\d+)[\'"]{0,1}', svg_line)
        if y1_match:
            y1 = int(y1_match.groups()[0])
        x2_match = re.search('x2=[\'"]{0,1}(\d+)[\'"]{0,1}', svg_line)
        if x2_match:
            x2 = int(x2_match.groups()[0])
        y2_match = re.search('y2=[\'"]{0,1}(\d+)[\'"]{0,1}', svg_line)
        if y2_match:
            y2 = int(y2_match.groups()[0])
        print([x1, y1, x2, y2])
        if all([x1, y1, x2 , y2]):
            coords.append({'x1': x1, 'y1': y1, 'x2': x2, 'y2': y2})
    return coords

def add_phantom_lines(coords):
    print('add phantom')
    print(coords)
    new_coords = []
    phantoms = []
    # always move from origin to coords with pen up
    if len(coords) > 0:
        phantoms.append({'x1':0,'y1':0, 'x2':coords[0]['x1'], 'y2':coords[0]['y1']})
        # rest of phantoms
        if len(coords) > 1:
            for i in range(len(coords) - 1):
                phantoms.append({'x1': coords[i]['x2'],
                                 'y1': coords[i]['y2'],
                                 'x2': coords[i+1]['x1'],
                                 'y2': coords[i+1]['y1']})
        total_moves = zip(phantoms, coords)
        for move_pair in total_moves:
            phantom = move_pair[0]
            phantom['pen'] = 'up'
            solid = move_pair[1]
            solid['pen'] = 'down'
            new_coords.append(phantom)
            new_coords.append(solid)
    return new_coords

def from_line_to_cmds(lines_with_phantom):
    cmds = []
    prev_line = {'x1':0, 'x2':1, 'y1':0, 'y2':0}
    cmds.append('set')
    print(lines_with_phantom)
    for line_with_phantom in lines_with_phantom:
        dx = line_with_phantom['x2'] - line_with_phantom['x1']
        dy = line_with_phantom['y2'] - line_with_phantom['y2']
        distance = math.sqrt(dx * dx + dy * dy)
        angle = calculate_angle([dx, dy],
                                [prev_line['x2'] - prev_line['x1'],
                                prev_line['y2'] - prev_line['y1']])

        if line_with_phantom['pen'] == 'up':
            cmds.append('pen up')
        else:
            cmds.append('pen down')
        cmds.append('move (%s, %s)' % (angle, distance))
        prev_line = line_with_phantom
    return cmds


def calculate_angle(vec0, vec1):
    vec0_len = math.sqrt(vec0[0] * vec0[0] + vec0[1] * vec0[1])
    vec1_len = math.sqrt(vec1[0] * vec1[0] + vec1[1] * vec1[1])
    dot_product = (vec0[0]*vec1[0] + vec0[1]*vec1[1])/vec0_len/vec1_len
    cross_product = (vec0[0]*vec1[1] - vec0[1]*vec1[0])/vec0_len/vec1_len
    angle_in_radian = math.acos(dot_product)
    angle_in_degree = angle_in_radian/0.0174532925
    if cross_product < 0:
        angle_in_degree = -1.0 * angle_in_degree
    return angle_in_degree

def from_svg_to_cmds(svg):
    lines = get_lines(svg)
    print('lines')
    print(lines)
    line_coords = get_line_coords(lines)
    moves = add_phantom_lines(line_coords)
    cmds = from_line_to_cmds(moves)
    return cmds

