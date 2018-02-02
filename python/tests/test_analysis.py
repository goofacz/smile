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
import matplotlib
from smile.analysis import *
from math import sqrt

# There's no need to display interactive plots
matplotlib.use('AGG')
plt.ioff()


class TestAnalysis(unittest.TestCase):
    def test_absolute_position_error_histogram(self):
        results = Results.create_array(3, 2)
        results[0, Results.BEGIN_TRUE_POSITION_2D] = (0, 0)
        results[0, Results.POSITION_2D] = (15, 0)
        results[0, Results.END_TRUE_POSITION_2D] = (20, 0)

        results[1, Results.BEGIN_TRUE_POSITION_2D] = (0, 0)
        results[1, Results.POSITION_2D] = (0, 5)
        results[1, Results.END_TRUE_POSITION_2D] = (0, 40)

        results[2, Results.BEGIN_TRUE_POSITION_2D] = (0, 0)
        results[2, Results.POSITION_2D] = (6, 6)
        results[2, Results.END_TRUE_POSITION_2D] = (20, 20)

        true_position, position_errors = absolute_position_error_histogram(results, return_intermediate_results=True)
        self.assertTupleEqual((3, 2), true_position.shape)
        np.testing.assert_equal((10, 0), true_position[0, :])
        np.testing.assert_equal((0, 20), true_position[1, :])
        np.testing.assert_equal((10, 10), true_position[2, :])

        self.assertTupleEqual((3, ), position_errors.shape)
        self.assertEqual(5, position_errors[0])
        self.assertEqual(15, position_errors[1])
        self.assertAlmostEqual(sqrt(2 * 10**2) - sqrt(2 * 6**2), position_errors[2], places=10)

    def test_absolute_position_error_histogram(self):
        results = Results.create_array(3, 2)
        results[0, Results.BEGIN_TRUE_POSITION_2D] = (0, 0)
        results[0, Results.POSITION_2D] = (15, 0)
        results[0, Results.END_TRUE_POSITION_2D] = (20, 0)

        results[1, Results.BEGIN_TRUE_POSITION_2D] = (0, 0)
        results[1, Results.POSITION_2D] = (0, 5)
        results[1, Results.END_TRUE_POSITION_2D] = (0, 40)

        results[2, Results.BEGIN_TRUE_POSITION_2D] = (0, 0)
        results[2, Results.POSITION_2D] = (6, 6)
        results[2, Results.END_TRUE_POSITION_2D] = (20, 20)

        true_position, position_errors = absolute_position_error_histogram(results, return_intermediate_results=True)
        self.assertTupleEqual((3, 2), true_position.shape)
        np.testing.assert_equal((10, 0), true_position[0, :])
        np.testing.assert_equal((0, 20), true_position[1, :])
        np.testing.assert_equal((10, 10), true_position[2, :])

        self.assertTupleEqual((3, ), position_errors.shape)
        self.assertEqual(5, position_errors[0])
        self.assertEqual(15, position_errors[1])
        self.assertAlmostEqual(sqrt(2 * 10**2) - sqrt(2 * 6**2), position_errors[2], places=10)

    def test_obtain_unique_results(self):
        results = Results.create_array(6, 2)
        results[0, :] = (2, 10, 15, 0, 40, 45, 0, 70, 75, 0, 123)
        results[1, :] = (2, 20, 25, 0, 50, 55, 0, 80, 85, 0, 123)
        results[2, :] = (2, 30, 35, 0, 60, 65, 0, 90, 95, 0, 123)
        results[3, :] = (2, 100, 0, 0, 200, 0, 0, 300, 0, 0, 456)
        results[4, :] = (2, 100, 0, 0, 200, 0, 0, 300, 0, 0, 456)
        results[5, :] = (2, 10, 11, 0, 12, 13, 0, 14, 15, 0, 789)

        unique_results = obtain_unique_results(results)
        self.assertTupleEqual((3, 11), unique_results.shape)

        self.assertEqual(123, unique_results[0, Results.MAC_ADDRESS])
        np.testing.assert_equal((20, 25), unique_results[0, Results.POSITION_2D])
        np.testing.assert_equal((50, 55), unique_results[0, Results.BEGIN_TRUE_POSITION_2D])
        np.testing.assert_equal((80, 85), unique_results[0, Results.END_TRUE_POSITION_2D])

        self.assertEqual(456, unique_results[1, Results.MAC_ADDRESS])
        np.testing.assert_equal((100, 0), unique_results[1, Results.POSITION_2D])
        np.testing.assert_equal((200, 0), unique_results[1, Results.BEGIN_TRUE_POSITION_2D])
        np.testing.assert_equal((300, 0), unique_results[1, Results.END_TRUE_POSITION_2D])

        self.assertEqual(789, unique_results[2, Results.MAC_ADDRESS])
        np.testing.assert_equal((10, 11), unique_results[2, Results.POSITION_2D])
        np.testing.assert_equal((12, 13), unique_results[2, Results.BEGIN_TRUE_POSITION_2D])
        np.testing.assert_equal((14, 15), unique_results[2, Results.END_TRUE_POSITION_2D])


if __name__ == '__main__':
    unittest.main()
