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


def doan_vesely(coordinates, distances):
    """
    S. Van Doan and J. Vesely, "The effectivity comparison of TDOA analytical solution methods,"
    2015 16th International Radar Symposium (IRS), Dresden, 2015, pp. 800-805.
    """
    # TODO propose better way for arguments checking
    assert (coordinates.shape == (3, 2))
    assert (distances.shape == (3,))

    coordinates = np.matrix(coordinates)
    distances = np.matrix(distances)
    distances -= distances[0, 0]

    L = distances[0, 1]
    R = distances[0, 2]
    Xl = coordinates[1, 0] - coordinates[0, 0]
    Yl = coordinates[1, 1] - coordinates[0, 1]
    Xr = coordinates[2, 0] - coordinates[0, 0]
    Yr = coordinates[2, 1] - coordinates[0, 1]

    A = -2 * np.matrix(((Xl, Yl),
                        (Xr, Yr)))

    B = np.matrix(((2 * L, L ** 2 - Xl ** 2 - Yl ** 2),
                   (2 * R, R ** 2 - Xr ** 2 - Yr ** 2)))

    tmp, _, _, _ = np.linalg.lstsq(A, B, rcond=None)
    a = tmp[0, 0] ** 2 + tmp[1, 0] ** 2 - 1
    b = 2 * (tmp[0, 0] * tmp[0, 1] + tmp[1, 0] * tmp[1, 1])
    c = tmp[0, 1] ** 2 + tmp[1, 1] ** 2

    K = np.max(np.real(np.roots((a, b, c))))

    X = tmp[0, 0] * K + tmp[0, 1] + coordinates[0, 0]
    Y = tmp[1, 0] * K + tmp[1, 1] + coordinates[0, 1]

    return np.asarray((X, Y))


def fang(coordinates, distances):
    """
    B. T. Fang, "Simple solutions for hyperbolic and related position fixes," in IEEE Transactions on Aerospace
    and Electronic Systems, vol. 26, no. 5, pp. 748-753, Sep 1990.
    """
    # TODO propose better way for arguments checking
    assert (coordinates.shape == (3, 2))
    assert (distances.shape == (3,))

    coordinates = np.matrix(coordinates)
    if coordinates[1, 1] != 0:
        raise ValueError('Second anchor has to lie along a first station baseline')

    distances = np.matrix(distances)
    distances -= distances[0, 0]

    R_ab = distances[0, 1]
    if np.isclose(R_ab, 0):
        raise ValueError('Solver doesn\'t accept R_ab equal zero')

    R_ac = distances[0, 2]
    c_x = coordinates[2, 0]
    c_y = coordinates[2, 1]

    b = np.abs(np.linalg.norm(coordinates[1, :] - coordinates[0, :]))
    c = np.sqrt(np.sum(np.power(coordinates[2, 0:2], 2)))

    g = ((R_ac * (b / R_ab)) - c_x) / c_y
    h = (np.power(c, 2) - np.power(R_ac, 2) + R_ac * R_ab * (1 - np.power(b / R_ab, 2))) / (2 * c_y)
    d = -(1 - np.power(b / R_ab, 2) + np.power(g, 2))
    e = b * (1 - np.power(b / R_ab, 2)) - 2 * g * h
    f = (np.power(R_ab, 2) / 4) * np.power(1 - np.power(b / R_ab, 2), 2) - np.power(h, 2)

    x = np.max(np.real(np.roots((d, e, f))))
    y = g * x + h

    return x, y
