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

from smile.algorithms.common import *


class TestSortDistances(unittest.TestCase):
    def test_2d_coordinates(self):
        reference_distances = np.asarray([1, 2, 3, 4])
        reference_coordinates = np.asarray([[10, 10], [20, 20], [30, 30], [40, 40]])
        distances = np.asarray([4, 2, 1, 3])
        coordinates = np.asarray([[40, 40], [20, 20], [10, 10], [30, 30]])

        coordinates, distances = sort_distances(coordinates, distances)
        np.testing.assert_equal(distances, reference_distances)
        np.testing.assert_equal(coordinates, reference_coordinates)

    def test_3d_coordinates(self):
        reference_distances = np.asarray([1, 2, 3, 4])
        reference_coordinates = np.asarray([[10, 10, 11], [20, 20, 22], [30, 30, 33], [40, 40, 44]])
        distances = np.asarray([4, 2, 1, 3])
        coordinates = np.asarray([[40, 40, 44], [20, 20, 22], [10, 10, 11], [30, 30, 33]])

        coordinates, distances = sort_distances(coordinates, distances)
        np.testing.assert_equal(distances, reference_distances)
        np.testing.assert_equal(coordinates, reference_coordinates)

    def test_different_entries_number(self):
        distances = np.asarray([4, 2, 1, 3])
        coordinates = np.asarray([[40, 40], [10, 10], [30, 30]])

        with self.assertRaises(AttributeError):
            sort_distances(coordinates, distances)

    def test_invalid_distances(self):
        distances = np.asarray([[4, 4], [1, 1], [3, 3]])
        coordinates = np.asarray([[40, 40], [10, 10], [30, 30]])

        with self.assertRaises(AttributeError):
            sort_distances(coordinates, distances)

    def test_invalid_coordinates(self):
        distances = np.asarray([4, 2, 3])
        coordinates_1d = np.asarray([4, 2, 3])
        with self.assertRaises(AttributeError):
            sort_distances(coordinates_1d, distances)

        coordinates_4d = np.asarray([[40, 40, 44, 44], [20, 20, 22, 22], [30, 30, 33, 33]])
        with self.assertRaises(AttributeError):
            sort_distances(coordinates_4d, distances)


class TestGenerateTofMeasurements(unittest.TestCase):
    def test_small_grid(self):
        anchor_coordinates = (np.asanyarray((0, 0)), np.asanyarray((0, 2)),
                              np.asanyarray((2, 0)), np.asanyarray((2, 1)))
        grid_size = 2
        grid_gap = 1

        reference_mobile_positions = (np.asanyarray((0, 0)), np.asanyarray((0, 1)), np.asanyarray((0, 2)),
                                      np.asanyarray((1, 0)), np.asanyarray((1, 1)), np.asanyarray((1, 2)),
                                      np.asanyarray((2, 0)), np.asanyarray((2, 1)), np.asanyarray((2, 2)))

        reference_tof_values = (np.asanyarray((0, 0.00059958, 0.00059958, 0.00067036)),
                                np.asanyarray((0.00029979, 0.00029979, 0.00067036, 0.00059958)),
                                np.asanyarray((0.00059958, 0, 0.00084794, 0.00067036)),
                                np.asanyarray((0.00029979, 0.00067036, 0.00029979, 0.00042397)),
                                np.asanyarray((0.00042397, 0.00042397, 0.00042397, 0.00029979)),
                                np.asanyarray((0.00067036, 0.00029979, 0.00067036, 0.00042397)),
                                np.asanyarray((0.00059958, 0.00084794, 0, 0.00029979)),
                                np.asanyarray((0.00067036, 0.00067036, 0.00029979, 0)),
                                np.asanyarray((0.00084794, 0.00059958, 0.00059958, 0.00029979)))

        mobile_positions, tof_values = zip(*generate_tof_measurements(anchor_coordinates, grid_size, grid_gap))
        np.testing.assert_equal(mobile_positions, reference_mobile_positions)
        np.testing.assert_almost_equal(tof_values, reference_tof_values)


class TestGenerateTdoaMeasurements(unittest.TestCase):
    def test_small_grid(self):
        anchor_coordinates = (np.asanyarray((0, 0)), np.asanyarray((0, 2)),
                              np.asanyarray((2, 0)), np.asanyarray((2, 1)))
        grid_size = 2
        grid_gap = 1

        reference_mobile_positions = (np.asanyarray((0, 0)), np.asanyarray((0, 1)), np.asanyarray((0, 2)),
                                      np.asanyarray((1, 0)), np.asanyarray((1, 1)), np.asanyarray((1, 2)),
                                      np.asanyarray((2, 0)), np.asanyarray((2, 1)), np.asanyarray((2, 2)))

        reference_tdoa_values = (np.asanyarray((0, 0.00059958, 0.00059958, 0.00067036)),
                                 np.asanyarray((0, 0, 0.00037056, 0.00029979)),
                                 np.asanyarray((0.00059958, 0, 0.00084794, 0.00067036)),
                                 np.asanyarray((0, 0.00037056, 0, 0.00012418)),
                                 np.asanyarray((0.00012418, 0.00012418, 0.00012418, 0)),
                                 np.asanyarray((0.00037056, 0, 0.00037056, 0.00012418)),
                                 np.asanyarray((0.00059958, 0.00084794, 0, 0.00029979)),
                                 np.asanyarray((0.00067036, 0.00067036, 0.00029979, 0)),
                                 np.asanyarray((0.00054815, 0.00029979, 0.00029979, 0)))

        mobile_positions, tdoa_values = zip(*generate_tdoa_measurements(anchor_coordinates, grid_size, grid_gap))
        np.testing.assert_equal(mobile_positions, reference_mobile_positions)
        np.testing.assert_almost_equal(tdoa_values, reference_tdoa_values)


if __name__ == '__main__':
    unittest.main()
