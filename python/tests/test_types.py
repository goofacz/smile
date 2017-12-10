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
from sympy.physics.units import second

from smile.types import *


class TestMACAddress(unittest.TestCase):
    def test_creation(self):
        MACAddress(12345678)
        with self.assertRaises(TypeError):
            MACAddress('12345678')
        with self.assertRaises(TypeError):
            MACAddress(12345678.7)

    def test_str(self):
        self.assertEqual("DE:AD:BE:EF:10:01", str(MACAddress(244837814046721)))

    def test_eq(self):
        self.assertEqual(MACAddress(244837814046721), MACAddress(244837814046721))
        self.assertNotEqual(MACAddress(244837814046721), MACAddress(944837814046721))


class TestTimestamp(unittest.TestCase):
    def test_create(self):
        Timestamp(123)
        with self.assertRaises(TypeError):
            Timestamp('5')
        with self.assertRaises(TypeError):
            Timestamp(5.3)

    def test_convert(self):
        timestamp = Timestamp(123, picoseconds)
        self.assertEqual(1.23e-10 * second, timestamp.convert_to(second))


if __name__ == '__main__':
    unittest.main()
