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

test_scenario_1_anchors_coordinates = np.asarray(((0, 5),  # Anchor 1
                                                  (5, 10),  # Anchor 2
                                                  (10, 5)))  # Anchor 3

test_scenario_1_tdoas = np.asarray((0,
                                    0,
                                    0))

test_scenario_1_mobile_position = np.asarray((5, 5))

# TEST SCENARIO no. 1
#
#   ^
#   |
#   |
#   |                   *                *(5, 9) anchor2
#   |*                   (5, 5) mobile
#   | (0, 4) anchor1
#   |                                  *
#   |                                   (9,2) anchor3
#   |---------------------------------->
#

test_scenario_2_anchors_coordinates = np.asarray(((0, 4),  # Anchor 1
                                                  (10, 5),  # Anchor 2
                                                  (9, 2)))  # Anchor 3

test_scenario_2_tdoas = np.asarray((np.abs(np.linalg.norm(np.asarray((5, 5)) - np.asarray((0, 4)))) - 5,
                                    0,
                                    np.abs(np.linalg.norm(np.asarray((5, 5)) - np.asarray((9, 2)))) - 5,))

test_scenario_2_mobile_position = np.asarray((5, 5))


class TestDoanVesely(unittest.TestCase):
    def test_scenario1(self):
        coordinates = test_scenario_1_anchors_coordinates
        distances = test_scenario_1_tdoas
        position = doan_vesely(coordinates, distances)
        np.testing.assert_almost_equal(position, test_scenario_1_mobile_position, decimal=7)

    def test_scenario2(self):
        coordinates = test_scenario_2_anchors_coordinates
        distances = test_scenario_2_tdoas
        position = doan_vesely(coordinates, distances)
        np.testing.assert_almost_equal(position, test_scenario_2_mobile_position, decimal=1)


if __name__ == '__main__':
    unittest.main()
