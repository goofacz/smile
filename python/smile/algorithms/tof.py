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

import numpy as np


def simple_least_squares(anchors_coordinates, distances):
    n = anchors_coordinates.shape[0]
    anchors_x = anchors_coordinates[:, 0]
    anchors_y = anchors_coordinates[:, 1]

    A = np.zeros((n - 1, 2))
    A[:, 0] = anchors_x[1:n]
    A[:, 0] -= anchors_x[0]
    A[:, 1] = anchors_y[1:n]
    A[:, 1] -= anchors_y[0]

    B = np.zeros((n - 1, 3))
    B[:, 0] = np.power(distances[0], 2)
    B[:, 1] = -np.power(distances[1:n], 2)
    B[:, 2] = np.sum(np.power(A, 2), axis=1)
    B = np.sum(B, axis=1)
    B /= 2

    position = np.linalg.solve(A, B)
    return position + anchors_coordinates[0, :]

