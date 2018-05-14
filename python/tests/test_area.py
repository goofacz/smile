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
from io import StringIO

import shapely.geometry as sg

from smile.area import *


class TestArea(unittest.TestCase):
    def test_invalid_json(self):
        data = StringIO('{ \
          "holes": null \
        }')

        with self.assertRaises(ValueError):
            Area.load_json(data)

    def test_correct_json(self):
        data = StringIO('{ \
          "vertices": [ \
            [0, 0], \
            [0, 75], \
            [75, 75], \
            [75, 0] \
          ], \
          "holes": [ \
            [ \
              [2, 2], \
              [3, 2], \
              [3, 3], \
              [3, 2] \
            ], \
            [ \
              [5, 5], \
              [6, 5], \
              [6, 6], \
              [6, 5] \
            ] \
          ] \
        }')

        area = Area.load_json(data)
        self.assertIsInstance(area, Area)

    def test_contains(self):
        data = StringIO('{ \
          "vertices": [ \
            [0, 0], \
            [0, 75], \
            [75, 75], \
            [75, 0] \
          ] \
        }')

        area = Area.load_json(data)

        inside_point = sg.Point(10, 10)
        self.assertTrue(area.contains(inside_point))

        close_to_edge_point = sg.Point(-1e-6, 5)
        self.assertTrue(area.contains(close_to_edge_point))
        self.assertFalse(area.contains(close_to_edge_point, atol=1e-7))

        outside_point = sg.Point(100, 100)
        self.assertFalse(area.contains(outside_point))


if __name__ == '__main__':
    unittest.main()
