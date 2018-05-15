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

import smile.area as sa


class TestArea(unittest.TestCase):
    def test_create_with_empty_json(self):
        empty_json = StringIO('')

        with self.assertRaises(ValueError):
            sa.Area(empty_json)

    def test_create_with_invalid_json(self):
        invalid_json = StringIO('{ \
          "vertices": [ \
            [ \
        }')

        with self.assertRaises(ValueError):
            sa.Area(invalid_json)

    def test_create_with_correct_json(self):
        correct_json = StringIO('{ \
          "vertices": [ \
            [0, 0], \
            [0, 75], \
            [75, 75], \
            [75, 0] \
          ] \
        }')

        area = sa.Area(correct_json)
        self.assertIsInstance(area, sa.Area)


class TestAreaContains(unittest.TestCase):
    reference_json = '{ \
      "vertices": [ \
        [0, 0], \
        [0, 75], \
        [75, 75], \
        [75, 0] \
      ] \
    }'

    def test_point_inside_area(self):
        area = sa.Area(StringIO(TestAreaContains.reference_json))

        inside_point = sg.Point(10, 10)
        self.assertTrue(area.contains(inside_point))

    def test_point_close_to_area(self):
        area = sa.Area(StringIO(TestAreaContains.reference_json))

        close_to_edge_point = sg.Point(-1e-6, 5)
        self.assertTrue(area.contains(close_to_edge_point))
        self.assertFalse(area.contains(close_to_edge_point, atol=1e-7))

    def test_point_outside_area(self):
        area = sa.Area(StringIO(TestAreaContains.reference_json))

        outside_point = sg.Point(100, 100)
        self.assertFalse(area.contains(outside_point))


if __name__ == '__main__':
    unittest.main()
