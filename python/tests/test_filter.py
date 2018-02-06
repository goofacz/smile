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

from smile.filter import Condition, Filter


class TestResults(unittest.TestCase):
    class BottomA(object):
        class Column(IntEnum):
            FIRST = 0

    class MiddleA(BottomA):
        class Column(IntEnum):
            SECOND = 1

    class TopA(MiddleA):
        class Column(IntEnum):
            THIRD = 2

    class B(object):
        class Column(IntEnum):
            FOURTH = 3

    class C(object):
        class Row(IntEnum):
            FIRST = 0

    data = np.array([[1, 20, 300, 4000],
                     [2, 30, 400, 5000],
                     [3, 40, 500, 6000],
                     [4, 50, 600, 7000],
                     [5, 60, 700, 8000],
                     [6, 70, 800, 9000],
                     [7, 80, 900, 0],
                     [8, 90, 0, 1000],
                     [9, 0, 100, 2000]])

    def test_append_column_type_validation(self):
        data_filter = Filter(TestResults.TopA)
        try:
            data_filter.append(Condition.EQUAL, TestResults.TopA.Column.THIRD, ())
        except TypeError as error:
            self.fail('Filter.append() raised unexpected: {0}'.format(str(error)))

        with self.assertRaises(TypeError):
            data_filter.append(Condition.EQUAL, TestResults.B.Column.FOURTH, ())

        with self.assertRaises(TypeError):
            data_filter.append(Condition.EQUAL, TestResults.C.Row.FIRST, ())

    def test_append_simple_cases(self):
        data_filter = Filter(TestResults.TopA)
        data_filter.append(Condition.EQUAL, TestResults.BottomA.Column.FIRST, 3)
        np.testing.assert_equal(data_filter.execute(TestResults.data), [[3, 40, 500, 6000]])

        data_filter = Filter(TestResults.TopA)
        data_filter.append(Condition.LESS_EQUAL, TestResults.MiddleA.Column.SECOND, 40)
        np.testing.assert_equal(data_filter.execute(TestResults.data), [[1, 20, 300, 4000],
                                                                        [2, 30, 400, 5000],
                                                                        [3, 40, 500, 6000],
                                                                        [9, 0, 100, 2000]])

        data_filter = Filter(TestResults.TopA)
        data_filter.append(Condition.LESS, TestResults.TopA.Column.THIRD, 200)
        np.testing.assert_equal(data_filter.execute(TestResults.data), [[8, 90, 0, 1000],
                                                                        [9, 0, 100, 2000]])

        data_filter = Filter(TestResults.TopA)
        data_filter.append(Condition.GREATER, TestResults.TopA.Column.THIRD, 700)
        np.testing.assert_equal(data_filter.execute(TestResults.data), [[6, 70, 800, 9000],
                                                                        [7, 80, 900, 0]])

        data_filter = Filter(TestResults.TopA)
        data_filter.append(Condition.GREATER_EQUAL, TestResults.TopA.Column.THIRD, 700)
        np.testing.assert_equal(data_filter.execute(TestResults.data), [[5, 60, 700, 8000],
                                                                        [6, 70, 800, 9000],
                                                                        [7, 80, 900, 0]])

        data_filter = Filter(TestResults.TopA)
        data_filter.append(Condition.IN, TestResults.TopA.Column.THIRD, (300, 100, 600))
        np.testing.assert_equal(data_filter.execute(TestResults.data), [[1, 20, 300, 4000],
                                                                        [4, 50, 600, 7000],
                                                                        [9, 0, 100, 2000]])

    def test_append_complex_cases(self):
        data_filter = Filter(TestResults.TopA)
        data_filter.append(Condition.GREATER_EQUAL, TestResults.BottomA.Column.FIRST, 3)
        data_filter.append(Condition.LESS, TestResults.MiddleA.Column.SECOND, 90)
        data_filter.append(Condition.IN, TestResults.TopA.Column.THIRD, (0, 700, 900))
        np.testing.assert_equal(data_filter.execute(TestResults.data), [[5, 60, 700, 8000],
                                                                        [7, 80, 900, 0]])

        data_filter = Filter(TestResults.TopA)
        data_filter.append(Condition.GREATER_EQUAL, TestResults.BottomA.Column.FIRST, 7)
        data_filter.append(Condition.LESS, TestResults.MiddleA.Column.SECOND, 50)
        data_filter.append(Condition.IN, TestResults.TopA.Column.THIRD, (0, 700, 900))
        self.assertEqual(data_filter.execute(TestResults.data).shape, (0, 4))


if __name__ == '__main__':
    unittest.main()
