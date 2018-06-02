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
    def setUp(self):
        self.result1 = Result()
        self.result1.mac_address = 112233445566
        self.result1.position_dimensions = 2
        self.result1.position_x = 1
        self.result1.position_y = 2
        self.result1.position_z = 3
        self.result1.begin_true_position_x = 4
        self.result1.begin_true_position_y = 5
        self.result1.begin_true_position_z = 6
        self.result1.end_true_position_x = 7
        self.result1.end_true_position_y = 8
        self.result1.end_true_position_z = 9

        self.result2 = Result()
        self.result2.mac_address = 212233445566
        self.result2.position_dimensions = 2
        self.result2.position_x = 10
        self.result2.position_y = 20
        self.result2.position_z = 30
        self.result2.begin_true_position_x = 40
        self.result2.begin_true_position_y = 50
        self.result2.begin_true_position_z = 60
        self.result2.end_true_position_x = 70
        self.result2.end_true_position_y = 80
        self.result2.end_true_position_z = 90

        self.result3 = Result()
        self.result3.mac_address = 312233445566
        self.result3.position_dimensions = 2
        self.result3.position_x = 100
        self.result3.position_y = 200
        self.result3.position_z = 300
        self.result3.begin_true_position_x = 400
        self.result3.begin_true_position_y = 500
        self.result3.begin_true_position_z = 600
        self.result3.end_true_position_x = 700
        self.result3.end_true_position_y = 800
        self.result3.end_true_position_z = 900

        self.result4 = Result()
        self.result4.mac_address = 112233445566
        self.result4.position_dimensions = 2
        self.result4.position_x = 2
        self.result4.position_y = 4
        self.result4.position_z = 6
        self.result4.begin_true_position_x = 8
        self.result4.begin_true_position_y = 10
        self.result4.begin_true_position_z = 12
        self.result4.end_true_position_x = 14
        self.result4.end_true_position_y = 16
        self.result4.end_true_position_z = 18

        self.result5 = Result()
        self.result5.mac_address = 212233445566
        self.result5.position_dimensions = 2
        self.result5.position_x = 15
        self.result5.position_y = 25
        self.result5.position_z = 35
        self.result5.begin_true_position_x = 45
        self.result5.begin_true_position_y = 55
        self.result5.begin_true_position_z = 65
        self.result5.end_true_position_x = 75
        self.result5.end_true_position_y = 85
        self.result5.end_true_position_z = 95

        self.reference_results = [self.result1, self.result2, self.result3, self.result4, self.result5]

    def test_squeeze_results_mean(self):
        results = Results.create_array(self.reference_results)

        unique_results = squeeze_results(results, grouping_method="mac_address", averaging_method="mean")
        self.assertTupleEqual((3, 11), unique_results.shape)
        self.assertEqual(112233445566, unique_results[0, "mac_address"])
        np.testing.assert_equal((1.5, 3), unique_results[0, "position_2d"])
        np.testing.assert_equal((6, 7.5), unique_results[0, "begin_true_position_2d"])
        np.testing.assert_equal((10.5, 12), unique_results[0, "end_true_position_2d"])
        self.assertEqual(212233445566, unique_results[1, "mac_address"])
        np.testing.assert_equal((12.5, 22.5), unique_results[1, "position_2d"])
        np.testing.assert_equal((42.5, 52.5), unique_results[1, "begin_true_position_2d"])
        np.testing.assert_equal((72.5, 82.5), unique_results[1, "end_true_position_2d"])
        self.assertEqual(312233445566, unique_results[2, "mac_address"])
        np.testing.assert_equal((100, 200), unique_results[2, "position_2d"])
        np.testing.assert_equal((400, 500), unique_results[2, "begin_true_position_2d"])
        np.testing.assert_equal((700, 800), unique_results[2, "end_true_position_2d"])

    def test_squeeze_results_max(self):
        results = Results.create_array(self.reference_results)

        unique_results = squeeze_results(results, grouping_method="mac_address", averaging_method="max")
        self.assertTupleEqual((3, 11), unique_results.shape)
        self.assertEqual(112233445566, unique_results[0, "mac_address"])
        np.testing.assert_equal((2, 4), unique_results[0, "position_2d"])
        np.testing.assert_equal((8, 10), unique_results[0, "begin_true_position_2d"])
        np.testing.assert_equal((14, 16), unique_results[0, "end_true_position_2d"])
        self.assertEqual(212233445566, unique_results[1, "mac_address"])
        np.testing.assert_equal((15, 25), unique_results[1, "position_2d"])
        np.testing.assert_equal((45, 55), unique_results[1, "begin_true_position_2d"])
        np.testing.assert_equal((75, 85), unique_results[1, "end_true_position_2d"])
        self.assertEqual(312233445566, unique_results[2, "mac_address"])
        np.testing.assert_equal((100, 200), unique_results[2, "position_2d"])
        np.testing.assert_equal((400, 500), unique_results[2, "begin_true_position_2d"])
        np.testing.assert_equal((700, 800), unique_results[2, "end_true_position_2d"])

    def test_squeeze_results_min(self):
        results = Results.create_array(self.reference_results)

        unique_results = squeeze_results(results, grouping_method="mac_address", averaging_method="min")
        self.assertTupleEqual((3, 11), unique_results.shape)
        self.assertEqual(112233445566, unique_results[0, "mac_address"])
        np.testing.assert_equal((1, 2), unique_results[0, "position_2d"])
        np.testing.assert_equal((4, 5), unique_results[0, "begin_true_position_2d"])
        np.testing.assert_equal((7, 8), unique_results[0, "end_true_position_2d"])
        self.assertEqual(212233445566, unique_results[1, "mac_address"])
        np.testing.assert_equal((10, 20), unique_results[1, "position_2d"])
        np.testing.assert_equal((40, 50), unique_results[1, "begin_true_position_2d"])
        np.testing.assert_equal((70, 80), unique_results[1, "end_true_position_2d"])
        self.assertEqual(312233445566, unique_results[2, "mac_address"])
        np.testing.assert_equal((100, 200), unique_results[2, "position_2d"])
        np.testing.assert_equal((400, 500), unique_results[2, "begin_true_position_2d"])
        np.testing.assert_equal((700, 800), unique_results[2, "end_true_position_2d"])


if __name__ == '__main__':
    unittest.main()
