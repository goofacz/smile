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

from smile.algorithms.tof import *

# Test scenarios contains following information
# - Anchors' coordinates in 2D
# - True tag position in 2D
# - True distances between tag and anchors

def test_scenario_1():
    mobile_position = np.asarray((1, 5))

    anchors_coordinates = np.asarray(((0, 0),  # Anchor 1
                                      (0, 100),  # Anchor 2
                                      (100, 100)))  # Anchor 3

    distances = mobile_position - anchors_coordinates
    distances = np.linalg.norm(distances, axis=1)
    distances = np.abs(distances)

    return anchors_coordinates, distances, mobile_position


def test_scenario_2():
    mobile_position = np.asarray((34, 85))

    anchors_coordinates = np.asarray(((0, 0),  # Anchor 1
                                      (0, 100),  # Anchor 2
                                      (100, 100)))  # Anchor 3

    distances = mobile_position - anchors_coordinates
    distances = np.linalg.norm(distances, axis=1)
    distances = np.abs(distances)

    return anchors_coordinates, distances, mobile_position


class TestSimpleLeastSquares(unittest.TestCase):
    def test_scenario_1(self):
        anchors_coordinates, distances, true_mobile_position = test_scenario_1()
        position = simple_least_squares(anchors_coordinates, distances)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

    def test_scenario_2(self):
        anchors_coordinates, distances, true_mobile_position = test_scenario_2()
        position = simple_least_squares(anchors_coordinates, distances)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)


class TestFoyTaylorSeries(unittest.TestCase):
    def test_scenario_1(self):
        coordinates, distances, true_mobile_position = test_scenario_1()
        # Check different initial guesses
        position = foy_taylor_series(coordinates, distances, np.array((0, 0)), expected_delta=0.00001)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

        position = foy_taylor_series(coordinates, distances, np.array((1, 5)), expected_delta=0.00001)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

        position = foy_taylor_series(coordinates, distances, np.array((-1, -5)), expected_delta=0.00000001)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

        position = foy_taylor_series(coordinates, distances, np.array((10, 10)), expected_delta=0.00000001)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

    def test_scenario_2(self):
        coordinates, distances, true_mobile_position = test_scenario_2()
        # Check different initial guesses
        position = foy_taylor_series(coordinates, distances, np.array((0, 0)), expected_delta=0.00000001)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

        position = foy_taylor_series(coordinates, distances, np.array((1, 5)), expected_delta=0.00000001)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

        position = foy_taylor_series(coordinates, distances, np.array((-1, -5)), expected_delta=0.00000001)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)

        position = foy_taylor_series(coordinates, distances, np.array((10, 10)), expected_delta=0.00000001)
        np.testing.assert_almost_equal(position, true_mobile_position, decimal=7)


if __name__ == '__main__':
    unittest.main()
