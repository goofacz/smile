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

# There's no need to display interactive plots
import matplotlib
matplotlib.use('AGG')

import unittest
import numpy as np
from smile.results import Results


class TestResults(unittest.TestCase):
    def test_load_csv(self):
        # Check create_array()
        self.assertTrue(isinstance(Results.create_array(7), np.ndarray))
        self.assertTupleEqual((17, 11), Results.create_array(17).shape)

        # Check field access
        np.testing.assert_equal((2, 2, 2), Results.create_array(3)[:, Results.POSITION_DIMENSIONS])
        np.testing.assert_equal((3, 3, 3), Results.create_array(3, 3)[:, Results.POSITION_DIMENSIONS])

        results = Results.create_array(1)
        results[0, :] = (2, 10, 20, 30, 55, 66, 77, 70, 80, 90, 123456789)

        self.assertEqual(10, results[0, Results.POSITION_X])
        self.assertEqual(20, results[0, Results.POSITION_Y])
        self.assertEqual(30, results[0, Results.POSITION_Z])
        np.testing.assert_equal((10, 20), results[0, Results.POSITION_2D])
        np.testing.assert_equal((10, 20, 30), results[0, Results.POSITION_3D])

        self.assertEqual(55, results[0, Results.BEGIN_TRUE_POSITION_X])
        self.assertEqual(66, results[0, Results.BEGIN_TRUE_POSITION_Y])
        self.assertEqual(77, results[0, Results.BEGIN_TRUE_POSITION_Z])
        np.testing.assert_equal((55, 66), results[0, Results.BEGIN_TRUE_POSITION_2D])
        np.testing.assert_equal((55, 66, 77), results[0, Results.BEGIN_TRUE_POSITION_3D])

        self.assertEqual(70, results[0, Results.END_TRUE_POSITION_X])
        self.assertEqual(80, results[0, Results.END_TRUE_POSITION_Y])
        self.assertEqual(90, results[0, Results.END_TRUE_POSITION_Z])
        np.testing.assert_equal((70, 80), results[0, Results.END_TRUE_POSITION_2D])
        np.testing.assert_equal((70, 80, 90), results[0, Results.END_TRUE_POSITION_3D])

        self.assertEqual(123456789, results[0, Results.MAC_ADDRESS])


if __name__ == '__main__':
    unittest.main()
