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

    def test_numpy_compatibility(self):
        # Run some numpy functions on Array to make sure they still work
        data = TestArray.Data([[1, 10, 100],
                               [2, 20, 200],
                               [3, 30, 300]])

        np.testing.assert_equal(np.sum(data, axis=0), [6, 60, 600])

        np.testing.assert_equal(data.T, [[1, 2, 3],
                                         [10, 20, 30],
                                         [100, 200, 300]])


if __name__ == '__main__':
    unittest.main()
