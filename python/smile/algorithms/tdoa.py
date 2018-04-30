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
import itertools


def verify_position(mobile_position, anchors_coordinates, tdoa_distances):
    new_distances = []
    for anchor_coordinates in anchors_coordinates:
        new_distances.append(npla.norm(anchor_coordinates - mobile_position))

    new_distances = np.asanyarray(new_distances) - min(new_distances)
    return np.allclose(tdoa_distances, new_distances, atol=1.e-4)


def _fang_forward_transformation(coordinates):
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


def _fang_backward_transformation(translation, angle, point):
    # The same as _fang_forward_transformation() but backward
    rotation = np.array(((np.cos(-angle), -np.sin(-angle)),
                         (np.sin(-angle), np.cos(-angle))))

    point = np.dot(rotation, point)
    return point + translation


def _fang_order_input(coordinates, distances):
    input_range = range(0, coordinates.shape[0])
    for indices in itertools.permutations(input_range, 3):
        indices = np.asarray(indices)
        tmp_distances = distances[indices]
        if tmp_distances[1] != 0:
            return coordinates[indices, :], tmp_distances

        if tmp_distances[0] != tmp_distances[1]:
            return coordinates[indices, :], tmp_distances - tmp_distances[0]

    raise ValueError('Cannot find right order of anchors, it looks that R_ab == R_ac == 0')


def doan_vesely(coordinates, distances, reorder_anchors=True):
    """
    S. Van Doan and J. Vesely, "The effectivity comparison of TDOA analytical solution methods,"
    2015 16th International Radar Symposium (IRS), Dresden, 2015, pp. 800-805.
    """
    if coordinates.shape != (3, 2):
        raise ValueError('Invalid shape of anchors coordinates array')
    if distances.shape != (3,):
        raise ValueError('Invalid shape of distances array')

    L = distances[1]
    R = distances[2]
    Xl = coordinates[1, 0] - coordinates[0, 0]
    Yl = coordinates[1, 1] - coordinates[0, 1]
    Xr = coordinates[2, 0] - coordinates[0, 0]
    Yr = coordinates[2, 1] - coordinates[0, 1]

    A = -2 * np.asarray(((Xl, Yl),
                         (Xr, Yr)))

    B = np.asarray(((2 * L, L ** 2 - Xl ** 2 - Yl ** 2),
                    (2 * R, R ** 2 - Xr ** 2 - Yr ** 2)))

    tmp, _, _, _ = npla.lstsq(A, B, rcond=None)
    a = tmp[0, 0] ** 2 + tmp[1, 0] ** 2 - 1
    b = 2 * (tmp[0, 0] * tmp[0, 1] + tmp[1, 0] * tmp[1, 1])
    c = tmp[0, 1] ** 2 + tmp[1, 1] ** 2

    positions = []
    for K in np.real(np.roots((a, b, c))):
        X = tmp[0, 0] * K + tmp[0, 1] + coordinates[0, 0]
        Y = tmp[1, 0] * K + tmp[1, 1] + coordinates[0, 1]
        positions.append((X, Y))

    return np.asarray(positions)


def fang(coordinates, distances, reorder_anchors=True):
    """
    B. T. Fang, "Simple solutions for hyperbolic and related position fixes," in IEEE Transactions on Aerospace
    and Electronic Systems, vol. 26, no. 5, pp. 748-753, Sep 1990.
    """

    if coordinates.shape != (3, 2):
        raise ValueError('Invalid shape of anchors coordinates array')
    if distances.shape != (3,):
        raise ValueError('Invalid shape of distances array')

    # Reorder anchors to have R_ab != 0 (see article)
    if reorder_anchors:
        coordinates, distances = _fang_order_input(coordinates, distances)

    # Transform coordinates to meet paper requirements
    translation, rotation, coordinates = _fang_forward_transformation(coordinates)
    if not np.isclose(coordinates[1, 1], coordinates[0, 1]):
        raise ValueError('Second anchor has to lie along a first station baseline')

    R_ab = distances[1]
    if np.isclose(R_ab, 0):
        raise ValueError('Solver doesn\'t accept R_ab equal zero')

    R_ac = distances[2]
    c_x = coordinates[2, 0]
    c_y = coordinates[2, 1]

    b = np.abs(npla.norm(coordinates[1, :] - coordinates[0, :]))
    c = np.sqrt(np.sum(np.power(coordinates[2, 0:2], 2)))

    g = ((R_ac * (b / R_ab)) - c_x) / c_y
    h = (np.power(c, 2) - np.power(R_ac, 2) + R_ac * R_ab * (1 - np.power(b / R_ab, 2))) / (2 * c_y)
    d = -(1 - np.power(b / R_ab, 2) + np.power(g, 2))
    e = b * (1 - np.power(b / R_ab, 2)) - 2 * g * h
    f = (np.power(R_ab, 2) / 4) * np.power(1 - np.power(b / R_ab, 2), 2) - np.power(h, 2)

    positions = []
    for x in np.real(np.roots((d, e, f))):
        y = g * x + h
        # Backward transformation of coordinates system
        positions.append(_fang_backward_transformation(translation, rotation, [x, y]))

    return positions


def chan_ho(coordinates, distances, reorder_anchors=True):
    """
    K. C. Ho and Y. T. Chan, "Solution and performance analysis of geolocation by TDOA," in IEEE Transactions
    on Aerospace and Electronic Systems, vol. 29, no. 4, pp. 1311-1322, Oct 1993.
    """

    if coordinates.shape != (4, 2):
        raise ValueError('Invalid shape of anchors coordinates array')
    if distances.shape != (4,):
        raise ValueError('Invalid shape of distances array')

    input_coordinates = coordinates.copy()
    input_distances = distances.copy()

    # Iterate over all possible anchors orders
    if reorder_anchors:
        input_range = range(0, input_coordinates.shape[0])
        all_indices = itertools.permutations(input_range, input_coordinates.shape[0])
    else:
        all_indices = [range(0, input_coordinates.shape[0])]

    for indices in all_indices:
        indices = np.asarray(indices)
        coordinates = input_coordinates[indices, :]
        distances = input_distances[indices]

        K = np.power(coordinates, 2)
        K = np.sum(K, 1)
        K_1 = K[0]
        K_2 = K[1]
        K_3 = K[2]
        K_4 = K[3]

        r_32 = distances[2] - distances[1]
        r_21 = distances[1] - distances[0]
        r_31 = distances[2] - distances[0]
        r_41 = distances[3] - distances[0]
        r_43 = distances[3] - distances[2]

        x_1 = coordinates[0, 0]
        x_2 = coordinates[1, 0]
        x_3 = coordinates[2, 0]
        x_4 = coordinates[3, 0]

        y_1 = coordinates[0, 1]
        y_2 = coordinates[1, 1]
        y_3 = coordinates[2, 1]
        y_4 = coordinates[3, 1]

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
            position = npla.solve(A1, B1.T)
        except npla.LinAlgError:
            continue

        # Another matrix for computing position
        # r_42 = distances[3] - distances[1]
        # l_3 = (r_42 * K_1) + (r_21 * K_4) - (r_41 * K_2)
        # l_4 = (r_43 * K_2) + (r_32 * K_4) - (r_42 * K_3)
        # m_3 = -2 * ((r_42 * x_1) + (r_21 * x_4) - (r_41 * x_2))
        # m_4 = -2 * ((r_43 * x_2) + (r_32 * x_4) - (r_42 * x_3))
        # u_3 = -2 * ((r_42 * y_1) + (r_21 * y_4) - (r_41 * y_2))
        # u_4 = -2 * ((r_43 * y_2) + (r_32 * y_4) - (r_42 * y_3))
        # A2 = np.asarray(((m_3, u_3),
        #                 (m_4, u_4)))
        # B2 = np.asarray(((np.float(r_42 * r_21 * r_41 - l_3)),
        #                 (np.float(r_43 * r_32 * r_42 - l_4))))
        # position2 = npla.solve(A2, B2.T)

        return position

    raise ValueError('Anchors coordinates always lead to singular A1 matrix')