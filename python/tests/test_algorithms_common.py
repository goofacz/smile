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


class TestSortMeasurements(unittest.TestCase):
    def test_2d_coordinates(self):
        reference_distances = np.asarray([1, 2, 3, 4])
        reference_coordinates = np.asarray([[10, 10], [20, 20], [30, 30], [40, 40]])
        distances = np.asarray([4, 2, 1, 3])
        coordinates = np.asarray([[40, 40], [20, 20], [10, 10], [30, 30]])

        coordinates, distances = sort_measurements(coordinates, distances)
        np.testing.assert_equal(distances, reference_distances)
        np.testing.assert_equal(coordinates, reference_coordinates)

    def test_3d_coordinates(self):
        reference_distances = np.asarray([1, 2, 3, 4])
        reference_coordinates = np.asarray([[10, 10, 11], [20, 20, 22], [30, 30, 33], [40, 40, 44]])
        distances = np.asarray([4, 2, 1, 3])
        coordinates = np.asarray([[40, 40, 44], [20, 20, 22], [10, 10, 11], [30, 30, 33]])

        coordinates, distances = sort_measurements(coordinates, distances)
        np.testing.assert_equal(distances, reference_distances)
        np.testing.assert_equal(coordinates, reference_coordinates)

    def test_different_entries_number(self):
        distances = np.asarray([4, 2, 1, 3])
        coordinates = np.asarray([[40, 40], [10, 10], [30, 30]])

        with self.assertRaises(ValueError):
            sort_measurements(coordinates, distances)

    def test_invalid_distances(self):
        distances = np.asarray([[4, 4], [1, 1], [3, 3]])
        coordinates = np.asarray([[40, 40], [10, 10], [30, 30]])

        with self.assertRaises(ValueError):
            sort_measurements(coordinates, distances)

    def test_invalid_coordinates(self):
        distances = np.asarray([4, 2, 3])
        coordinates_1d = np.asarray([4, 2, 3])
        with self.assertRaises(ValueError):
            sort_measurements(coordinates_1d, distances)

        coordinates_4d = np.asarray([[40, 40, 44, 44], [20, 20, 22, 22], [30, 30, 33, 33]])
        with self.assertRaises(ValueError):
            sort_measurements(coordinates_4d, distances)


class TestGenerateTofMeasurements(unittest.TestCase):
    def test_small_grid(self):
        anchor_coordinates = np.asanyarray(((0, 0), (0, 2), (2, 0), (2, 1)))
        grid_size = 2
        grid_gap = 1

        reference_mobile_positions = (np.asanyarray((0, 0)),
                                      np.asanyarray((0, 1)),
                                      np.asanyarray((1, 0)),
                                      np.asanyarray((1, 1)))

        reference_tof_values = (np.asanyarray((0, 2, 2, 2.23606798)),
                                np.asanyarray((1, 1, 2.23606798, 2)),
                                np.asanyarray((1, 2.23606798, 1, 1.41421356)),
                                np.asanyarray((1.41421356, 1.41421356, 1.41421356, 1.)))

        mobile_positions, tof_values = zip(*generate_tof_measurements(anchor_coordinates, grid_size, grid_gap))
        np.testing.assert_equal(mobile_positions, reference_mobile_positions)
        np.testing.assert_almost_equal(tof_values, reference_tof_values)

    def test_invalid_anchors_coordinates(self):
        grid_size = 2
        grid_gap = 1

        with self.assertRaises(ValueError):
            list(*generate_tof_measurements(np.asanyarray(()), grid_size, grid_gap))

        with self.assertRaises(ValueError):
            list(*generate_tof_measurements(np.asanyarray((1)), grid_size, grid_gap))

        with self.assertRaises(ValueError):
            list(*generate_tof_measurements(np.asanyarray((1, 2, 4, 5)), grid_size, grid_gap))

    def test_invalid_grid(self):
        anchor_coordinates = np.asanyarray([(0, 0)])

        with self.assertRaises(ValueError):
            zip(*generate_tof_measurements(anchor_coordinates, -2, 1))

        with self.assertRaises(ValueError):
            zip(*generate_tof_measurements(anchor_coordinates, 0, 1))

        with self.assertRaises(ValueError):
            zip(*generate_tof_measurements(anchor_coordinates, 3, 0))

        with self.assertRaises(ValueError):
            zip(*generate_tof_measurements(anchor_coordinates, 3, 3))

        with self.assertRaises(ValueError):
            zip(*generate_tof_measurements(anchor_coordinates, 3, 5))


class TestGenerateTdoaMeasurements(unittest.TestCase):
    def test_small_grid(self):
        reference_anchor_coordinates = np.asanyarray(((0, 0), (0, 2), (2, 0), (2, 1)))
        grid_size = 2
        grid_gap = 1

        reference_mobile_positions = (np.asanyarray((0, 0)),
                                      np.asanyarray((0, 1)),
                                      np.asanyarray((1, 0)),
                                      np.asanyarray((1, 1)))

        reference_tdoa_values = (np.asanyarray((0, 2, 2, 2.23606798)),
                                 np.asanyarray((0, 0, 1.23606798, 1)),
                                 np.asanyarray((0, 1.23606798, 0, 0.41421356)),
                                 np.asanyarray((0.41421356, 0.41421356, 0.41421356, 0)))

        mobile_positions, tdoa_values = zip(*generate_tdoa_measurements(reference_anchor_coordinates, grid_size, grid_gap))
        np.testing.assert_equal(mobile_positions, reference_mobile_positions)
        np.testing.assert_almost_equal(tdoa_values, reference_tdoa_values)


class TestDoesAreaContainPosition(unittest.TestCase):
    def test_positive_cases(self):
        top_left_cornet = (0, 5)
        bottom_right_cornet = (10, 0)

        self.assertTrue(does_area_contain_position((0, 0), top_left_cornet, bottom_right_cornet))
        self.assertTrue(does_area_contain_position((10, 5), top_left_cornet, bottom_right_cornet))
        self.assertTrue(does_area_contain_position((2, 4), top_left_cornet, bottom_right_cornet))
        self.assertTrue(does_area_contain_position((10.00000000001, 5), top_left_cornet, bottom_right_cornet))

    def test_negative_cases(self):
        top_left_cornet = (0, 5)
        bottom_right_cornet = (10, 0)

        self.assertFalse(does_area_contain_position((0, -2), top_left_cornet, bottom_right_cornet))
        self.assertFalse(does_area_contain_position((11, 5), top_left_cornet, bottom_right_cornet))
        self.assertFalse(does_area_contain_position((10, 5.001), top_left_cornet, bottom_right_cornet))


if __name__ == '__main__':
    unittest.main()
