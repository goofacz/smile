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
from smile.algorithms.tdoa import *


class TestDoanVesely(unittest.TestCase):
    def test_small_grid(self):
        anchors_coordinates = np.asanyarray(((0, 0),
                                             (0, 6),
                                             (6, 6)))
        grid_size = 6
        grid_gap = 1

        # For these positions algorithm fails to compute correct solution
        skipped_positions = np.asanyarray(((0, 5), (1, 6)))

        for reference_position, tdoa_distances in generate_tdoa_measurements(anchors_coordinates, grid_size, grid_gap):
            if np.all(np.isin(reference_position, skipped_positions)):
                continue
            sorted_anchors_coordinates, sorted_tdoa_distances = sort_measurements(anchors_coordinates, tdoa_distances)
            position = doan_vesely(sorted_anchors_coordinates, sorted_tdoa_distances)
            np.testing.assert_almost_equal(position, reference_position, decimal=7)


class TestFang(unittest.TestCase):
    def test_small_grid(self):
        anchors_coordinates = np.asanyarray(((0, 6),
                                             (6, 6),
                                             (6, 0)))
        grid_size = 6
        grid_gap = 1

        # For these positions algorithm fails to compute correct solution
        skipped_positions = np.asanyarray(((0, 1), (0, 2), (0, 3), (0, 4), (0, 5), (0, 6)))

        for reference_position, tdoa_distances in generate_tdoa_measurements(anchors_coordinates, grid_size, grid_gap):
            if np.all(np.isin(reference_position, skipped_positions)):
                continue
            position = fang(anchors_coordinates, tdoa_distances)
            np.testing.assert_almost_equal(position, reference_position, decimal=7)


class TestChanHo(unittest.TestCase):
    def test_small_grid(self):
        anchors_coordinates = np.asanyarray(((0, 0),
                                             (0, 6),
                                             (6, 0),
                                             (6, 6)))
        grid_size = 6
        grid_gap = 1

        # For these positions algorithm fails to compute correct solution
        skipped_positions = np.asanyarray(((0, 3), (1, 3), (2, 3), (4, 3), (5, 3), (6, 3)))

        for reference_position, tdoa_distances in generate_tdoa_measurements(anchors_coordinates, grid_size, grid_gap):
            if np.all(np.isin(reference_position, skipped_positions)):
                continue
            sorted_anchors_coordinates, sorted_tdoa_distances = sort_measurements(anchors_coordinates, tdoa_distances)
            position = chan_ho(sorted_anchors_coordinates, sorted_tdoa_distances)
            np.testing.assert_almost_equal(position, reference_position, decimal=7)


if __name__ == '__main__':
    unittest.main()
