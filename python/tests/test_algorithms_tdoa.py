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


def _is_skipped_position(reference_position, skipped_positions):
    for skipped_position in skipped_positions:
        if np.array_equal(reference_position, skipped_position):
            return True

    return False


class TestDoanVesely(unittest.TestCase):
    def test_small_grid(self):
        anchors_coordinates = np.asanyarray(((0, 0),
                                             (0, 6),
                                             (6, 6)))
        grid_size = 6
        grid_gap = 1

        # For these positions algorithm fails to compute correct solution
        skipped_positions = [(0, 5), (1, 6)]

        for reference_position, tdoa_distances in generate_tdoa_measurements(anchors_coordinates, grid_size, grid_gap):
            if _is_skipped_position(reference_position, skipped_positions):
                continue
            sorted_anchors_coordinates, sorted_tdoa_distances = sort_measurements(anchors_coordinates, tdoa_distances)
            position = doan_vesely(sorted_anchors_coordinates, sorted_tdoa_distances)
            np.testing.assert_almost_equal(position, reference_position, decimal=7)


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

            positions = fang(sorted_anchors_coordinates, sorted_tdoa_distances)

            if np.allclose(positions[0], positions[1]):
                positions = [positions[0]]
            positions = [position for position in positions if does_area_contain_position(position, top_left,bottom_right)]
            positions = [position for position in positions if verify_position(position, sorted_anchors_coordinates, sorted_tdoa_distances)]

            self.assertEqual(1, len(positions), msg=error_message)
            np.testing.assert_almost_equal(positions[0], reference_position, decimal=7, err_msg=error_message)


class TestChanHo(unittest.TestCase):
    def test_positive_cases(self):
        anchors_coordinates = np.asanyarray(((1, 0),
                                             (0, 6),
                                             (6, 0),
                                             (6, 6)))
        grid_size = 6
        grid_gap = 1

        for reference_position, tdoa_distances in generate_tdoa_measurements(anchors_coordinates, grid_size, grid_gap):
            error_message = 'Reference position: ({0}, {1})'.format(*reference_position)

            position = chan_ho(anchors_coordinates, tdoa_distances)
            np.testing.assert_almost_equal(position, reference_position, decimal=7, err_msg=error_message)

    def test_invalid_anchors_coordinates(self):
        # Reference position is (0, 3)
        anchors_coordinates = np.asarray(((0, 0),
                                          (0, 6),
                                          (6, 0),
                                          (6, 6)))

        tdoa_distances = np.asarray((0., 0., 3.70820393, 3.70820393))

        with self.assertRaises(ValueError):
            chan_ho(anchors_coordinates, tdoa_distances)


class TestVerifyPosition(unittest.TestCase):
    def test_out_of_order_anchors(self):
        # reference_mobile_positions is ((1, 0))
        reference_anchor_coordinates = np.asanyarray(((0, 2), (0, 0), (2, 0), (2, 1)))
        reference_tdoa_values = np.asanyarray((1.23606798, 0, 0, 0.41421356))

        self.assertTrue(verify_position((1, 0), reference_anchor_coordinates, reference_tdoa_values))
        self.assertTrue(verify_position((1.000000001, 0), reference_anchor_coordinates, reference_tdoa_values))
        self.assertTrue(verify_position((1, -0.0000001), reference_anchor_coordinates, reference_tdoa_values))

        self.assertFalse(verify_position((1, 1), reference_anchor_coordinates, reference_tdoa_values))
        self.assertFalse(verify_position((6, -1), reference_anchor_coordinates, reference_tdoa_values))

    def test_ordered_anchors(self):
        # reference_mobile_positions is ((1, 0))
        reference_anchor_coordinates = np.asanyarray(((0, 0), (2, 0), (2, 1), (0, 2)))
        reference_tdoa_values = np.asanyarray((0, 0, 0.41421356, 1.23606798))

        self.assertTrue(verify_position((1, 0), reference_anchor_coordinates, reference_tdoa_values))
        self.assertTrue(verify_position((1.000000001, 0), reference_anchor_coordinates, reference_tdoa_values))
        self.assertTrue(verify_position((1, -0.0000001), reference_anchor_coordinates, reference_tdoa_values))

        self.assertFalse(verify_position((1, 1), reference_anchor_coordinates, reference_tdoa_values))
        self.assertFalse(verify_position((6, -1), reference_anchor_coordinates, reference_tdoa_values))



if __name__ == '__main__':
    unittest.main()
