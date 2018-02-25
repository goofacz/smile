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
from smile.analysis import *
import numpy as np
from smile.results import Results


class TestAnalysis(unittest.TestCase):
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

        self.assertEqual(123, unique_results[0, "mac_address"])
        np.testing.assert_equal((20, 25), unique_results[0, "position_2d"])
        np.testing.assert_equal((50, 55), unique_results[0, "begin_true_position_2d"])
        np.testing.assert_equal((80, 85), unique_results[0, "end_true_position_2d"])

        self.assertEqual(456, unique_results[1, "mac_address"])
        np.testing.assert_equal((100, 0), unique_results[1, "position_2d"])
        np.testing.assert_equal((200, 0), unique_results[1, "begin_true_position_2d"])
        np.testing.assert_equal((300, 0), unique_results[1, "end_true_position_2d"])

        self.assertEqual(789, unique_results[2, "mac_address"])
        np.testing.assert_equal((10, 11), unique_results[2, "position_2d"])
        np.testing.assert_equal((12, 13), unique_results[2, "begin_true_position_2d"])
        np.testing.assert_equal((14, 15), unique_results[2, "end_true_position_2d"])


if __name__ == '__main__':
    unittest.main()
