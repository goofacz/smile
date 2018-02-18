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

import unittest

from smile.algorithms.tdoa import *
import scipy.constants as scc


def test_scenario_1():
    # TEST SCENARIO no. 1
    #
    #   ^
    #   |                    *
    #   |                     (5, 10) anchor2
    #   |
    #   |
    #   |
    #   |*                   *                  *
    #   | (0, 5) anchor1      (5, 5) mobile      (10,5) anchor3
    #   |
    #   |
    #   |
    #   |---------------------------------->
    #
    assert (scc.unit('speed of light in vacuum') == 'm s^-1')
    c = scc.value('speed of light in vacuum')

    mobile_position = np.asarray((5, 5))

    coordinates = np.asarray(((0, 5),  # Anchor 1
                              (5, 10),  # Anchor 2
                              (10, 5)))  # Anchor 3

    distances = np.asarray((np.abs(np.linalg.norm(mobile_position - coordinates[0, :])),
                            np.abs(np.linalg.norm(mobile_position - coordinates[1, :])),
                            np.abs(np.linalg.norm(mobile_position - coordinates[2, :]))))
    tdoas = distances / c

    return coordinates, tdoas, mobile_position

def test_scenario_2():
    # TEST SCENARIO no. 1
    #
    #   ^
    #   *                                   *
    #   |(0,10 anchor1                       (10,10) anchor2
    #   |
    #   |
    #   |                   *
    #   |                    (5, 5) mobile
    #   |
    #   |
    #   |                                     (10,0) anchor3
    #   |------------------------------------*--->
    #
    assert (scc.unit('speed of light in vacuum') == 'm s^-1')
    c = scc.value('speed of light in vacuum')

    mobile_position = np.asarray((5, 5))

    coordinates = np.asarray(((0, 0),  # Anchor 1
                              (10, 10),  # Anchor 2
                              (10, 0)))  # Anchor 3

    distances = np.asarray((np.abs(np.linalg.norm(mobile_position - coordinates[0,:])),
                            np.abs(np.linalg.norm(mobile_position - coordinates[1,:])),
                            np.abs(np.linalg.norm(mobile_position - coordinates[2,:]))))

    tdoas = distances / c

    return coordinates, tdoas, mobile_position


class TestDoanVesely(unittest.TestCase):
    def test_scenario_1(self):
        coordinates, distances, true_mobile_position = test_scenario_1()
        position = doan_vesely(coordinates, distances)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

    def test_scenario_2(self):
        coordinates, distances, true_mobile_position = test_scenario_2()
        position = doan_vesely(coordinates, distances)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)


if __name__ == '__main__':
    unittest.main()
