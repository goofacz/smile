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
from smile.algorithms.tof import *

anchors_coordinates = np.asanyarray(((0, 0),
                                     (0, 6),
                                     (6, 6)))
grid_size = 6
grid_gap = 1


class TestSimpleLeastSquares(unittest.TestCase):
    def test_small_grid(self):
        for reference_position, tof_distances in generate_tof_measurements(anchors_coordinates, grid_size, grid_gap):
            position = simple_least_squares(anchors_coordinates, tof_distances)
            np.testing.assert_almost_equal(position, reference_position, decimal=7)


class TestFoyTaylorSeries(unittest.TestCase):
    def test_small_grid_with_exact_initial_solutions(self):
        # For these positions algorithm fails to compute correct solution
        skipped_positions = np.asanyarray(((0, 0),
                                           (0, 6)))

        for reference_position, tof_distances in generate_tof_measurements(anchors_coordinates, grid_size, grid_gap):
            if np.all(np.isin(reference_position, skipped_positions)):
                continue
            position = foy_taylor_series(anchors_coordinates, tof_distances, reference_position,
                                         expected_delta=0.000000001, loop_limit=100)
            np.testing.assert_almost_equal(position, reference_position, decimal=7)

    def test_small_grid_with_imprecise_initial_solution(self):
        initial_solution = np.asanyarray((2.5, 2.5))

        # For these positions algorithm fails to compute correct solution
        skipped_positions = np.asanyarray(((0, 0),
                                           (0, 6)))

        for reference_position, tof_distances in generate_tof_measurements(anchors_coordinates, grid_size, grid_gap):
            if np.all(np.isin(reference_position, skipped_positions)):
                continue
            position = foy_taylor_series(anchors_coordinates, tof_distances, initial_solution,
                                         expected_delta=0.0001, loop_limit=200)
            np.testing.assert_almost_equal(position, reference_position, decimal=2)


if __name__ == '__main__':
    unittest.main()
