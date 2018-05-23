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
from smile.algorithms.tdoa.fang import *


def _is_skipped_position(reference_position, skipped_positions):
    for skipped_position in skipped_positions:
        if np.array_equal(reference_position, skipped_position):
            return True

    return False


class TestFang(unittest.TestCase):
    def test_small_grid(self):
        anchors_coordinates = np.asanyarray(((0, 0),
                                             (6, 6),
                                             (6, 0)))
        grid_size = 6
        grid_gap = 1

        # Area boundaries
        top_left = (0, 5)
        bottom_right = (5, 0)

        # For these positions algorithm fails to compute correct solution
        skipped_positions = [(3, 3)]

        for reference_position, tdoa_distances in generate_tdoa_measurements(anchors_coordinates, grid_size, grid_gap):
            if _is_skipped_position(reference_position, skipped_positions):
                continue

            sorted_anchors_coordinates, sorted_tdoa_distances = sort_measurements(anchors_coordinates, tdoa_distances)
            error_message = 'Reference position: ({0}, {1})'.format(*reference_position)

            solver = Fang(sorted_anchors_coordinates, sorted_tdoa_distances, {})
            positions = solver.localize()

            if len(positions) > 1 and np.allclose(positions[0], positions[1]):
                positions = [positions[0]]
            positions = [position for position in positions if does_area_contain_position(position, top_left,bottom_right)]

            self.assertEqual(1, len(positions), msg=error_message)
            np.testing.assert_almost_equal(positions[0], reference_position, decimal=7, err_msg=error_message)


if __name__ == '__main__':
    unittest.main()
