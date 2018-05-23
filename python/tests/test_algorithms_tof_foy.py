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
from smile.algorithms.tof.foy import *

anchors_coordinates = np.asanyarray(((0, 0),
                                     (0, 6),
                                     (6, 6)))
grid_size = 6
grid_gap = 1

configuration = {'initial_position': (0, 0), 'R': None, 'expected_delta': 0.000000001, 'loop_limit': 100}


def _is_skipped_position(reference_position, skipped_positions):
    for skipped_position in skipped_positions:
        if np.array_equal(reference_position, skipped_position):
            return True

    return False


class TestFoy(unittest.TestCase):
    def test_small_grid_with_exact_initial_solutions(self):
        # For these positions algorithm fails to compute correct solution
        skipped_positions = [(0, 0)]

        for reference_position, tof_distances in generate_tof_measurements(anchors_coordinates, grid_size, grid_gap):
            if _is_skipped_position(reference_position, skipped_positions):
                continue

            error_message = 'Reference position: ({0}, {1})'.format(*reference_position)
            solver = Foy(anchors_coordinates, tof_distances, configuration, initial_position=reference_position)
            positions = solver.localize()
            np.testing.assert_almost_equal(positions[0], reference_position, decimal=7, err_msg=error_message)

    def test_small_grid_with_imprecise_initial_solution(self):
        initial_solution = np.asanyarray((2.5, 2.5))

        # For these positions algorithm fails to compute correct solution
        skipped_positions = [(0, 0)]

        for reference_position, tof_distances in generate_tof_measurements(anchors_coordinates, grid_size, grid_gap):
            if _is_skipped_position(reference_position, skipped_positions):
                continue

            error_message = 'Reference position: ({0}, {1})'.format(*reference_position)
            solver = Foy(anchors_coordinates, tof_distances, configuration, initial_position=initial_solution,
                         expected_delta=0.0001, loop_limit=200)
            positions = solver.localize()
            np.testing.assert_almost_equal(positions[0], reference_position, decimal=2, err_msg=error_message)

    def test_invalid_to_distances(self):
        # Reference point is at (0, 0)
        tof_distances = np.asarray((0, 6, 8.48528137))

        with self.assertRaises(ValueError):
            solver = Foy(anchors_coordinates, tof_distances, configuration)
            solver.localize()


if __name__ == '__main__':
    unittest.main()
