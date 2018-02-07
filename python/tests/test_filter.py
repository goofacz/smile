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
from enum import IntEnum

import numpy as np

from smile.array import Array
from smile.filter import Condition, Filter


class TestFilter(unittest.TestCase):
    class BottomA(Array):
        class Column(IntEnum):
            FIRST = 0

    class MiddleA(BottomA):
        class Column(IntEnum):
            SECOND = 1

    class TopA(MiddleA):
        class Column(IntEnum):
            THIRD = 2

    class B(Array):
        class Column(IntEnum):
            FOURTH = 3

    class C(object):
        class Row(IntEnum):
            FIRST = 0

    data = TopA([[1, 20, 300, 4000],
                 [2, 30, 400, 5000],
                 [3, 40, 500, 6000],
                 [4, 50, 600, 7000],
                 [5, 60, 700, 8000],
                 [6, 70, 800, 9000],
                 [7, 80, 900, 0],
                 [8, 90, 0, 1000],
                 [9, 0, 100, 2000]])

    def test_append_column_validation(self):
        # Correct case
        try:
            data_filter = Filter(TestFilter.TopA)
            data_filter.append(Condition.EQUAL, TestFilter.TopA.Column.THIRD, ())
        except TypeError as error:
            self.fail('Filter.append() raised unexpected: {0}'.format(str(error)))

        # TestFilter.B and TestFilter.TopA are not related
        with self.assertRaises(TypeError):
            data_filter = Filter(TestFilter.TopA)
            data_filter.append(Condition.EQUAL, TestFilter.B.Column.FOURTH, ())

        # TestFilter.C doesn't have Column enumeration
        with self.assertRaises(TypeError):
            data_filter = Filter(TestFilter.TopA)
            data_filter.append(Condition.EQUAL, TestFilter.C.Row.FIRST, ())

    def test_execute_array_type_validation(self):
        # Construct & run on the same array class
        try:
            data_filter = Filter(TestFilter.TopA)
            data_filter.execute(TestFilter.TopA(()))
        except TypeError as error:
            self.fail('Filter.execute() raised unexpected: {0}'.format(str(error)))

        # Construct for base array class & run on the derived array class
        try:
            data_filter = Filter(TestFilter.BottomA)
            data_filter.execute(TestFilter.TopA(()))
        except TypeError as error:
            self.fail('Filter.execute() raised unexpected: {0}'.format(str(error)))

        # Construct & run using different array classes
        with self.assertRaises(TypeError):
            data_filter = Filter(TestFilter.TopA)
            data_filter.execute(TestFilter.B(()))

    def test_execute_equal_condition(self):
        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.EQUAL, TestFilter.BottomA.Column.FIRST, 3)
        np.testing.assert_equal(data_filter.execute(TestFilter.data), [[3, 40, 500, 6000]])

    def test_execute_less_equal_condition(self):
        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.LESS_EQUAL, TestFilter.MiddleA.Column.SECOND, 40)
        np.testing.assert_equal(data_filter.execute(TestFilter.data), [[1, 20, 300, 4000],
                                                                       [2, 30, 400, 5000],
                                                                       [3, 40, 500, 6000],
                                                                       [9, 0, 100, 2000]])

    def test_execute_less_condition(self):
        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.LESS, TestFilter.TopA.Column.THIRD, 200)
        np.testing.assert_equal(data_filter.execute(TestFilter.data), [[8, 90, 0, 1000],
                                                                       [9, 0, 100, 2000]])

    def test_execute_greater_condition(self):
        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.GREATER, TestFilter.TopA.Column.THIRD, 700)
        np.testing.assert_equal(data_filter.execute(TestFilter.data), [[6, 70, 800, 9000],
                                                                       [7, 80, 900, 0]])

    def test_execute_greater_equal_condition(self):
        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.GREATER_EQUAL, TestFilter.TopA.Column.THIRD, 700)
        np.testing.assert_equal(data_filter.execute(TestFilter.data), [[5, 60, 700, 8000],
                                                                       [6, 70, 800, 9000],
                                                                       [7, 80, 900, 0]])

    def test_execute_in_condition(self):
        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.IN, TestFilter.TopA.Column.THIRD, (300, 100, 600))
        np.testing.assert_equal(data_filter.execute(TestFilter.data), [[1, 20, 300, 4000],
                                                                       [4, 50, 600, 7000],
                                                                       [9, 0, 100, 2000]])

    def test_execute_complex_cases(self):
        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.GREATER_EQUAL, TestFilter.BottomA.Column.FIRST, 3)
        data_filter.append(Condition.LESS, TestFilter.MiddleA.Column.SECOND, 90)
        data_filter.append(Condition.IN, TestFilter.TopA.Column.THIRD, (0, 700, 900))
        np.testing.assert_equal(data_filter.execute(TestFilter.data), [[5, 60, 700, 8000],
                                                                       [7, 80, 900, 0]])

        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.GREATER_EQUAL, TestFilter.BottomA.Column.FIRST, 7)
        data_filter.append(Condition.LESS, TestFilter.MiddleA.Column.SECOND, 50)
        data_filter.append(Condition.IN, TestFilter.TopA.Column.THIRD, (0, 700, 900))
        self.assertEqual(data_filter.execute(TestFilter.data).shape, (0, 4))

    def test_execute_return_copy(self):
        data_filter = Filter(TestFilter.TopA)
        data_filter.append(Condition.IN, TestFilter.TopA.Column.THIRD, (300, 100, 600))
        self.assertIsNotNone(data_filter.execute(TestFilter.data).base)
        self.assertIsNone(data_filter.execute(TestFilter.data, copy=True).base, None)


if __name__ == '__main__':
    unittest.main()
