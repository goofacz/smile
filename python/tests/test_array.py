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

from smile.array import Array


class TestArray(unittest.TestCase):
    class Data(Array):
        def __init__(self, *args):
            super(self.__class__, self).__init__()
            self.column_names["first"] = 0
            self.column_names["second"] = 1
            self.column_names["third"] = 2

    def test_construction(self):
        TestArray.Data([0, 1, 2])

    def test_getitem(self):
        data = TestArray.Data([[1, 10, 100],
                               [2, 20, 200],
                               [3, 30, 300]])

        np.testing.assert_equal(data[:, :], [[1, 10, 100],
                                             [2, 20, 200],
                                             [3, 30, 300]])

        np.testing.assert_equal(data[1, :], [2, 20, 200])

        np.testing.assert_equal(data[:, 2], [100, 200, 300])

        np.testing.assert_equal(data[:, (0, 2)], [[1, 100],
                                                  [2, 200],
                                                  [3, 300]])

        np.testing.assert_equal(data["third"], [100, 200, 300])

        self.assertEqual(data[2, "second"], 30)

        np.testing.assert_equal(data[:, "first"], [1, 2, 3])

    def test_setitem(self):
        reference_data = TestArray.Data([[1, 10, 100],
                                         [2, 20, 200],
                                         [3, 30, 300]])

        data = reference_data.copy()
        data[:, :] = [[5, 50, 500],
                      [6, 60, 600],
                      [7, 70, 700]]
        np.testing.assert_equal(data, [[5, 50, 500],
                                       [6, 60, 600],
                                       [7, 70, 700]])

        data = reference_data.copy()
        data[1, :] = (7, 8, 9)
        np.testing.assert_equal(data, [[1, 10, 100],
                                       [7, 8, 9],
                                       [3, 30, 300]])

        data = reference_data.copy()
        data[:, 1] = (7, 8, 9)
        np.testing.assert_equal(data, [[1, 7, 100],
                                       [2, 8, 200],
                                       [3, 9, 300]])

        data = reference_data.copy()
        data[:, "second"] = (11, 12, 13)
        np.testing.assert_equal(data, [[1, 11, 100],
                                       [2, 12, 200],
                                       [3, 13, 300]])
        data = reference_data.copy()
        data["third"] = (71, 72, 73)
        np.testing.assert_equal(data, [[1, 10, 71],
                                       [2, 20, 72],
                                       [3, 30, 73]])

    def test_numpy_compatibility(self):
        # Run some numpy functions on Array to make sure they still work
        reference_a = TestArray.Data([[1, 10, 100],
                                      [2, 20, 200],
                                      [3, 30, 300]])

        reference_b = np.array([[5, 60, 700],
                               [5, 60, 700],
                               [5, 60, 700]])

        a = reference_a.copy()
        np.testing.assert_equal(np.sum(a, axis=0), [6, 60, 600])

        a = reference_a.copy()
        np.testing.assert_equal(a.T, [[1, 2, 3],
                                         [10, 20, 30],
                                         [100, 200, 300]])


if __name__ == '__main__':
    unittest.main()
