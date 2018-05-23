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

from smile.algorithms.solver import Solver


class Foy(Solver):
    """
    W. H. Foy, "Position-Location Solutions by Taylor-Series Estimation," in IEEE Transactions on Aerospace
    and Electronic Systems, vol. AES-12, no. 2, pp. 187-194, March 1976.
    """

    def __init__(self, anchors_coordinates, distances, configuration, initial_position=None, R=None,
                 expected_delta=None, loop_limit=None):
        super().__init__(anchors_coordinates, distances, configuration)
        self.tof_distances = distances

        if initial_position is None:
            self.initial_position = self.configuration['initial_position']
        else:
            self.initial_position = initial_position

        if R is None:
            self.R = self.configuration['R']
        else:
            self.R = R

        if expected_delta is None:
            self.expected_delta = self.configuration['expected_delta']
        else:
            self.expected_delta = expected_delta

        if loop_limit is None:
            self.loop_limit = self.configuration['loop_limit']
        else:
            self.loop_limit = loop_limit

    def localize(self):
        if self.anchors_coordinates.shape != (3, 2):
            raise ValueError('Invalid shape of anchors coordinates array')
        if self.tof_distances.shape[0] != 3:
            raise ValueError('Invalid shape of distances array')

        if len(self.tof_distances.shape) == 1:
            distances = self.tof_distances[np.newaxis]

        if np.count_nonzero(distances) != distances.shape[1]:
            raise ValueError('All ToF distances have to be nonzero values')

        if self.R is None:
            self.R = np.ones(distances.shape)

        A = self.initial_position - self.anchors_coordinates
        A = A / distances.T

        self.R = np.diagflat(self.R)
        invR = np.linalg.inv(self.R)

        position = self.initial_position

        for _ in range(self.loop_limit):
            D = self.anchors_coordinates - position
            D = np.power(D, 2)
            D = np.sum(D, 1)
            D = np.sqrt(D)
            D = distances - D.T
            delta = np.linalg.inv(A.T @ invR @ A) @ A.T @ invR @ D.T
            position = position + delta.T

            if np.allclose(np.abs(delta), (self.expected_delta, self.expected_delta)):
                break

        return [np.squeeze(position)]
