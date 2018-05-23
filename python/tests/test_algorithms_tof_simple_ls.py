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
from smile.algorithms.tof.simple_ls import *

anchors_coordinates = np.asanyarray(((0, 0),
                                     (0, 6),
                                     (6, 6)))
grid_size = 6
grid_gap = 1


def _is_skipped_position(reference_position, skipped_positions):
    for skipped_position in skipped_positions:
        if np.array_equal(reference_position, skipped_position):
            return True

    return False


class TestSimpleLeastSquares(unittest.TestCase):
    def test_small_grid(self):
        for reference_position, tof_distances in generate_tof_measurements(anchors_coordinates, grid_size, grid_gap):
            solver = SimpleLs(anchors_coordinates, tof_distances, {})
            positions = solver.localize()
            np.testing.assert_almost_equal(positions[0], reference_position, decimal=7)


if __name__ == '__main__':
    unittest.main()
