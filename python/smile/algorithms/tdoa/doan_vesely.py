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
import numpy.linalg as npla

from smile.algorithms.solver import Solver


class DoanVesely(Solver):
    """
    S. Van Doan and J. Vesely, "The effectivity comparison of TDOA analytical solution methods,"
    2015 16th International Radar Symposium (IRS), Dresden, 2015, pp. 800-805.
    """

    def __init__(self, anchors_coordinates, distances, configuration):
        super().__init__(anchors_coordinates, distances, configuration)
        self.tdoa_distances = distances

    def localize(self):
        if self.anchors_coordinates.shape != (3, 2):
            raise ValueError('Invalid shape of anchors coordinates array')
        if self.tdoa_distances.shape != (3,):
            raise ValueError('Invalid shape of distances array')

        L = self.tdoa_distances[1]
        R = self.tdoa_distances[2]
        Xl = self.anchors_coordinates[1, 0] - self.anchors_coordinates[0, 0]
        Yl = self.anchors_coordinates[1, 1] - self.anchors_coordinates[0, 1]
        Xr = self.anchors_coordinates[2, 0] - self.anchors_coordinates[0, 0]
        Yr = self.anchors_coordinates[2, 1] - self.anchors_coordinates[0, 1]

        A = -2 * np.asarray(((Xl, Yl, 0),
                             (Xr, Yr, 0)))

        B = np.asarray(((2 * L, L ** 2 - Xl ** 2 - Yl ** 2),
                        (2 * R, R ** 2 - Xr ** 2 - Yr ** 2)))

        tmp, _, _, _ = npla.lstsq(A, B, rcond=None)
        a = tmp[0, 0] ** 2 + tmp[1, 0] ** 2 - 1
        b = 2 * (tmp[0, 0] * tmp[0, 1] + tmp[1, 0] * tmp[1, 1])
        c = tmp[0, 1] ** 2 + tmp[1, 1] ** 2

        positions = []
        roots = np.real(np.roots((a, b, c)))
        positive_roots = [root for root in roots if root >= 0]

        for K in positive_roots:
            X = tmp[0, 0] * K + tmp[0, 1] + self.anchors_coordinates[0, 0]
            Y = tmp[1, 0] * K + tmp[1, 1] + self.anchors_coordinates[0, 1]

            # Article eq. (10)
            K_condition = np.asarray([npla.norm(X - self.anchors_coordinates[0, 0]),
                                      npla.norm(Y - self.anchors_coordinates[0, 1])])
            K_condition = K_condition ** 2
            K_condition = np.sum(K_condition)
            K_condition = np.sqrt(K_condition)
            if not np.isclose(K, K_condition):
                continue

            solution = np.asarray((X, Y))
            positions.append(solution)

        if not len(positions):
            raise ValueError('Failed to compute solution')

        return positions
