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
import numpy as np
from smile.results import Results


class TestResults(unittest.TestCase):
    def test_create(self):
        # Check create_array()
        results = Results.create_array(3)
        self.assertTrue(isinstance(results, Results))
        self.assertTupleEqual((3, 11), results.shape)

        results = Results.create_array(2, position_dimensions=3, mac_address=11223344)
        self.assertTrue(isinstance(results, Results))
        np.testing.assert_equal(results, [[3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11223344],
                                          [3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11223344]])

        # Check field access
        results = Results.create_array(3)
        np.testing.assert_equal((2, 2, 2), results["position_dimensions"])

        results = Results.create_array(3, 3)
        np.testing.assert_equal((3, 3, 3), results["position_dimensions"])

        results = Results.create_array(1)
        results[0, :] = (2, 10, 20, 30, 55, 66, 77, 70, 80, 90, 123456789)

        self.assertEqual(10, results[0, "position_x"])
        self.assertEqual(20, results[0, "position_y"])
        self.assertEqual(30, results[0, "position_z"])
        np.testing.assert_equal((10, 20), results[0, "position_2d"])
        np.testing.assert_equal((10, 20, 30), results[0, "position_3d"])

        self.assertEqual(55, results[0, "begin_true_position_x"])
        self.assertEqual(66, results[0, "begin_true_position_y"])
        self.assertEqual(77, results[0, "begin_true_position_z"])
        np.testing.assert_equal((55, 66), results[0, "begin_true_position_2d"])
        np.testing.assert_equal((55, 66, 77), results[0, "begin_true_position_3d"])

        self.assertEqual(70, results[0, "end_true_position_x"])
        self.assertEqual(80, results[0, "end_true_position_y"])
        self.assertEqual(90, results[0, "end_true_position_z"])
        np.testing.assert_equal((70, 80), results[0, "end_true_position_2d"])
        np.testing.assert_equal((70, 80, 90), results[0, "end_true_position_3d"])

        self.assertEqual(123456789, results[0, "mac_address"])


if __name__ == '__main__':
    unittest.main()
