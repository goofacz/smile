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


class ChanHo(Solver):
    """
    Y. T. Chan and K. C. Ho, "A simple and efficient estimator for hyperbolic location," in IEEE Transactions on Signal
    Processing, vol. 42, no. 8, pp. 1905-1915, Aug 1994.

    K. C. Ho and Y. T. Chan, "Solution and performance analysis of geolocation by TDOA," in IEEE Transactions
    on Aerospace and Electronic Systems, vol. 29, no. 4, pp. 1311-1322, Oct 1993.
    """

    def __init__(self, anchors_coordinates, distances, configuration):
        super().__init__(anchors_coordinates, distances, configuration)
        self.tdoa_distances = self.distances

    def __three_anchors_case(self):
        r_21 = self.tdoa_distances[1] - self.tdoa_distances[0]
        r_31 = self.tdoa_distances[2] - self.tdoa_distances[0]

        x_2 = self.anchors_coordinates[1, 0] - self.anchors_coordinates[0, 0]
        x_3 = self.anchors_coordinates[2, 0] - self.anchors_coordinates[0, 0]

        y_2 = self.anchors_coordinates[1, 1] - self.anchors_coordinates[0, 1]
        y_3 = self.anchors_coordinates[2, 1] - self.anchors_coordinates[0, 1]

        K_2 = x_2 ** 2 + y_2 ** 2
        K_3 = x_3 ** 2 + y_3 ** 2

        A = np.asarray(((x_2, y_2),
                        (x_3, y_3)))

        B = np.asarray((-r_21,
                        -r_31))

        C = 0.5 * np.asarray(((-(r_21 ** 2) + K_2),
                              (-(r_31 ** 2) + K_3)))

        try:
            D = npla.solve(A, B)
            E = npla.solve(A, C)
        except npla.LinAlgError:
            raise ValueError('Anchors coordinates always lead to singular A matrix')

        a = np.sum(D * D) - 1
        b = 2 * np.sum(E * D)
        c = np.sum(E * E)

        results = np.real(np.roots((a, b, c)))
        results = [result for result in results if result > 0]

        positions = []
        for result in results:
            position = D * result + E
            position += self.anchors_coordinates[0]

            # Avoid adding very close (duplicate?) positions
            add_position = True
            for other_position in positions:
                positions_distance = npla.norm(position - other_position)
                if positions_distance < 0.01:  # TODO Replace magic number with method argument?
                    add_position = False
                    break

            if add_position:
                positions.append(position)

        return positions

    def __four_anchors_case(self):
        K = np.power(self.anchors_coordinates, 2)
        K = np.sum(K, 1)
        K_1 = K[0]
        K_2 = K[1]
        K_3 = K[2]
        K_4 = K[3]

        r_32 = self.tdoa_distances[2] - self.tdoa_distances[1]
        r_21 = self.tdoa_distances[1] - self.tdoa_distances[0]
        r_31 = self.tdoa_distances[2] - self.tdoa_distances[0]
        r_41 = self.tdoa_distances[3] - self.tdoa_distances[0]
        r_43 = self.tdoa_distances[3] - self.tdoa_distances[2]

        x_1 = self.anchors_coordinates[0, 0]
        x_2 = self.anchors_coordinates[1, 0]
        x_3 = self.anchors_coordinates[2, 0]
        x_4 = self.anchors_coordinates[3, 0]

        y_1 = self.anchors_coordinates[0, 1]
        y_2 = self.anchors_coordinates[1, 1]
        y_3 = self.anchors_coordinates[2, 1]
        y_4 = self.anchors_coordinates[3, 1]

        l_1 = (r_32 * K_1) + (r_21 * K_3) - (r_31 * K_2)
        l_2 = (r_43 * K_1) + (r_31 * K_4) - (r_41 * K_3)

        m_1 = -2 * ((r_32 * x_1) + (r_21 * x_3) - (r_31 * x_2))
        m_2 = -2 * ((r_43 * x_1) + (r_31 * x_4) - (r_41 * x_3))

        u_1 = -2 * ((r_32 * y_1) + (r_21 * y_3) - (r_31 * y_2))
        u_2 = -2 * ((r_43 * y_1) + (r_31 * y_4) - (r_41 * y_3))

        A1 = np.asarray(((m_1, u_1),
                         (m_2, u_2)))

        B1 = np.asarray(((np.float(r_32 * r_21 * r_31 - l_1)),
                         (np.float(r_43 * r_31 * r_41 - l_2))))

        try:
            # If A1 is singular matrix solve() will fail, and we just try to reorder anchors
            position = npla.solve(A1, B1)
        except npla.LinAlgError:
            raise ValueError('Anchors coordinates always lead to singular A1 matrix')

        # Another matrix for computing position
        # r_42 = distances[3] - distances[1]
        # l_3 = (r_42 * K_1) + (r_21 * K_4) - (r_41 * K_2)
        # l_4 = (r_43 * K_2) + (r_32 * K_4) - (r_42 * K_3)
        # m_3 = -2 * ((r_42 * x_1) + (r_21 * x_4) - (r_41 * x_2))
        # m_4 = -2 * ((r_43 * x_2) + (r_32 * x_4) - (r_42 * x_3))
        # u_3 = -2 * ((r_42 * y_1) + (r_21 * y_4) - (r_41 * y_2))
        # u_4 = -2 * ((r_43 * y_2) + (r_32 * y_4) - (r_42 * y_3))
        # A2 = np.asarray(((m_3, u_3),
        #                  (m_4, u_4)))
        # B2 = np.asarray(((np.float(r_42 * r_21 * r_41 - l_3)),
        #                  (np.float(r_43 * r_32 * r_42 - l_4))))

        # position = npla.solve(A2, B2)

        return [position]

    def localize(self):
        if self.anchors_coordinates.shape[0] != self.tdoa_distances.shape[0]:
            raise ValueError('Number of distances has to match number of anchors')

        if self.anchors_coordinates.shape == (3, 2):
            return self.__three_anchors_case()
        elif self.anchors_coordinates.shape == (4, 2):
            return self.__four_anchors_case()
        else:
            raise ValueError('Invalid shape of anchors coordinates array')
