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
from smile.analysis import *
from math import sqrt

# There's no need to display plots
plt.show = lambda: None


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


if __name__ == '__main__':
    unittest.main()
