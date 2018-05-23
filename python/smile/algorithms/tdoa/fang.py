#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http:#www.gnu.org/licenses/.
#

import itertools

import numpy as np
import numpy.linalg as npla

from smile.algorithms.solver import Solver


class Fang(Solver):
    """
    B. T. Fang, "Simple solutions for hyperbolic and related position fixes," in IEEE Transactions on Aerospace
    and Electronic Systems, vol. 26, no. 5, pp. 748-753, Sep 1990.
    """

    def __init__(self, anchors_coordinates, distances, configuration):
        super().__init__(anchors_coordinates, distances, configuration)
        self.tdoa_distances = distances

    def __forward_transformation(self, coordinates):
        # Translate to anchors coordinates to put first anchor at (0, 0)
        translation = coordinates[0, :].copy()
        coordinates -= translation

        B = coordinates[1, :].copy()
        C = coordinates[2, :].copy()

        # Rotate coordinates system to make sure two first anchors lie on X a axis
        angle = np.arctan2(*B) - np.pi / 2
        rotation = np.array(((np.cos(angle), -np.sin(angle)),
                             (np.sin(angle), np.cos(angle))))

        B = np.dot(rotation, B)
        C = np.dot(rotation, C)

        return translation, angle, np.asarray(((0, 0), B, C))

    def __backward_transformation(self, translation, angle, point):
        # The same as _fang_forward_transformation() but backward
        rotation = np.array(((np.cos(-angle), -np.sin(-angle)),
                             (np.sin(-angle), np.cos(-angle))))

        point = np.dot(rotation, point)
        return point + translation

    def __order_input(self, coordinates, distances):
        input_range = range(0, coordinates.shape[0])
        for indices in itertools.permutations(input_range, 3):
            indices = np.asarray(indices)
            tmp_distances = distances[indices]
            if tmp_distances[1] != 0:
                return coordinates[indices, :], tmp_distances

            if tmp_distances[0] != tmp_distances[1]:
                return coordinates[indices, :], tmp_distances - tmp_distances[0]

        raise ValueError('Cannot find right order of anchors, it looks that R_ab == R_ac == 0')

    def localize(self):
        if self.anchors_coordinates.shape != (3, 2):
            raise ValueError('Invalid shape of anchors coordinates array')
        if self.tdoa_distances.shape != (3,):
            raise ValueError('Invalid shape of distances array')

        # Reorder anchors to have R_ab != 0 (see article)
        anchors_coordinates, distances = self.__order_input(self.anchors_coordinates, self.tdoa_distances)

        # Transform coordinates to meet paper requirements
        translation, rotation, anchors_coordinates = self.__forward_transformation(anchors_coordinates)
        if not np.isclose(anchors_coordinates[1, 1], anchors_coordinates[0, 1]):
            raise ValueError('Second anchor has to lie along a first station baseline')

        R_ab = distances[1]
        if np.isclose(R_ab, 0):
            raise ValueError('Solver doesn\'t accept R_ab equal zero')

        R_ac = distances[2]
        c_x = anchors_coordinates[2, 0]
        c_y = anchors_coordinates[2, 1]

        b = npla.norm(anchors_coordinates[1, :] - anchors_coordinates[0, :])
        c = np.sqrt(np.sum(np.power(anchors_coordinates[2, 0:2], 2)))

        g = ((R_ac * (b / R_ab)) - c_x) / c_y
        h = (np.power(c, 2) - np.power(R_ac, 2) + R_ac * R_ab * (1 - np.power(b / R_ab, 2))) / (2 * c_y)
        d = -(1 - np.power(b / R_ab, 2) + np.power(g, 2))
        e = b * (1 - np.power(b / R_ab, 2)) - 2 * g * h
        f = (np.power(R_ab, 2) / 4) * np.power(1 - np.power(b / R_ab, 2), 2) - np.power(h, 2)

        positions = []

        try:
            roots = np.real(np.roots((d, e, f)))
        except npla.LinAlgError:
            raise ValueError('Cannot find roots of polynomial')

        for X in roots:
            Y = g * X + h

            # Verify result
            tmp_R_ab = np.sqrt(X ** 2 + Y ** 2) - np.sqrt((X - b) ** 2 + Y ** 2)
            tmp_R_ac = np.sqrt(X ** 2 + Y ** 2) - np.sqrt((X - c_x) ** 2 + (Y - c_y) ** 2)

            if np.isclose(R_ab, -tmp_R_ab) and np.isclose(R_ac, -tmp_R_ac):
                # Backward transformation of coordinates system
                solution = self.__backward_transformation(translation, rotation, [X, Y])
                positions.append(solution)

        return positions
