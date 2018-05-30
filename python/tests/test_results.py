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
from copy import deepcopy

import numpy as np

from smile.results import Results, Result


class TestResult(unittest.TestCase):
    def setUp(self):
        self.result = Result()
        self.result.mac_address = 112233445566
        self.result.position_dimensions = 2
        self.result.position_x = 10
        self.result.position_y = 20
        self.result.position_z = 30
        self.result.begin_true_position_x = 400
        self.result.begin_true_position_y = 500
        self.result.begin_true_position_z = 600
        self.result.end_true_position_x = 7000
        self.result.end_true_position_y = 8000
        self.result.end_true_position_z = 9000

    def test_to_list_success(self):
        reference_tuple = (2,
                           10,
                           20,
                           30,
                           400,
                           500,
                           600,
                           7000,
                           8000,
                           9000,
                           112233445566)

        self.assertSequenceEqual(self.result.to_tuple(), reference_tuple)

    def test_to_list_catch_nones(self):
        none_result = Result()
        with self.assertRaises(ValueError):
            none_result.to_tuple()

        none_result = deepcopy(self.result)
        none_result.position_z = None
        with self.assertRaises(ValueError):
            none_result.to_tuple()

        none_result = deepcopy(self.result)
        none_result.begin_true_position_z = None
        with self.assertRaises(ValueError):
            none_result.to_tuple()


class TestResults(unittest.TestCase):
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

        self.reference_results = [self.result1, self.result2, self.result3]

    def test_create_array_success(self):
        results = Results.create_array(self.reference_results)

        np.testing.assert_array_equal(results[0, :], self.result1.to_tuple())
        np.testing.assert_array_equal(results[1, :], self.result2.to_tuple())
        np.testing.assert_array_equal(results[2, :], self.result3.to_tuple())

    def test_create_array_different_position_dimensions(self):
        self.result1.position_dimensions = 2
        self.result2.position_dimensions = 3
        self.result3.position_dimensions = 2

        self.reference_results = [self.result1, self.result2, self.result3]

        with self.assertRaises(ValueError):
            Results.create_array(self.reference_results)


if __name__ == '__main__':
    unittest.main()
