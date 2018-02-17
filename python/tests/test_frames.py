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

import numpy as np

from smile.frames import Frames


class TestFrames(unittest.TestCase):
    content = \
        "12345678,RX,105000927418,205000927418,37.500000,87.500000,79.000000,305010927418,405010927418,137.500000,127.500000,46.000000,17592186044420,281474976710655,1\n" \
        "98765432,TX,140001177591,140001177591,37.500000,37.500000,0.000000,140011177591,140011177591,37.500000,37.500000,0.000000,17592186044417,281474976710655,2"

    def test_load_csv(self):
        frames = Frames.load_csv(StringIO(TestFrames.content))
        self.assertTrue(isinstance(frames, np.ndarray))
        self.assertTupleEqual((2, 15), frames.shape)

        # Check load_csv() faults
        with self.assertRaises(ValueError):
            Frames.load_csv(None)

        with self.assertRaises(OSError):
            Frames.load_csv("nonexisting file")

    def test_column_access(self):
        frames = Frames.load_csv(StringIO(TestFrames.content))

        np.testing.assert_equal((hash('RX'), hash('TX')), frames["direction"])

        self.assertEqual(12345678, frames[0, "node_mac_address"])
        self.assertEqual(98765432, frames[1, "node_mac_address"])

        self.assertEqual(105000927418, frames[0, "begin_clock_timestamp"])
        self.assertEqual(205000927418, frames[0, "begin_simulation_timestamp"])

        self.assertEqual(37.5, frames[0, "begin_true_position_x"])
        self.assertEqual(87.5, frames[0, "begin_true_position_y"])
        self.assertEqual(79, frames[0, "begin_true_position_z"])
        np.testing.assert_equal((37.5, 87.5), frames[0, "begin_true_position_2d"])
        np.testing.assert_equal((37.5, 87.5, 79), frames[0, "begin_true_position_3d"])

        self.assertEqual(305010927418, frames[0, "end_clock_timestamp"])
        self.assertEqual(405010927418, frames[0, "end_simulation_timestamp"])

        self.assertEqual(137.5, frames[0, "end_true_position_x"])
        self.assertEqual(127.5, frames[0, "end_true_position_y"])
        self.assertEqual(46, frames[0, "end_true_position_z"])
        np.testing.assert_equal((137.5, 127.5), frames[0, "end_true_position_2d"])
        np.testing.assert_equal((137.5, 127.5, 46), frames[0, "end_true_position_3d"])

        self.assertEqual(17592186044420, frames[0, "source_mac_address"])
        self.assertEqual(281474976710655, frames[0, "destination_mac_address"])

        self.assertEqual(1, frames[0, "sequence_number"])


if __name__ == '__main__':
    unittest.main()
