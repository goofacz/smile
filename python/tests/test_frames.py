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
    def test_load_csv(self):
        content = StringIO(
            "RX,105000927418,205000927418,37.500000,87.500000,79.000000,305010927418,405010927418,137.500000,127.500000,46.000000,17592186044420,281474976710655,1\nTX,140001177591,140001177591,37.500000,37.500000,0.000000,140011177591,140011177591,37.500000,37.500000,0.000000,17592186044417,281474976710655,2")

        # Check whether data is loaded into correct array
        nodes = Frames.load_csv(content)
        self.assertTrue(isinstance(nodes, np.ndarray))
        self.assertTupleEqual((2, 14), nodes.shape)

        # Check field access
        np.testing.assert_equal((hash('RX'), hash('TX')), nodes[:, Frames.DIRECTION])

        self.assertEqual(105000927418, nodes[0, Frames.BEGIN_CLOCK_TIMESTAMP])
        self.assertEqual(205000927418, nodes[0, Frames.BEGIN_SIMULATION_TIMESTAMP])

        self.assertEqual(37.5, nodes[0, Frames.BEGIN_TRUE_POSITION_X])
        self.assertEqual(87.5, nodes[0, Frames.BEGIN_TRUE_POSITION_Y])
        self.assertEqual(79, nodes[0, Frames.BEGIN_TRUE_POSITION_Z])
        np.testing.assert_equal((37.5, 87.5), nodes[0, Frames.BEGIN_TRUE_POSITION_2D])
        np.testing.assert_equal((37.5, 87.5, 79), nodes[0, Frames.BEGIN_TRUE_POSITION_3D])

        self.assertEqual(305010927418, nodes[0, Frames.END_CLOCK_TIMESTAMP])
        self.assertEqual(405010927418, nodes[0, Frames.END_SIMULATION_TIMESTAMP])

        self.assertEqual(137.5, nodes[0, Frames.END_TRUE_POSITION_X])
        self.assertEqual(127.5, nodes[0, Frames.END_TRUE_POSITION_Y])
        self.assertEqual(46, nodes[0, Frames.END_TRUE_POSITION_Z])
        np.testing.assert_equal((137.5, 127.5), nodes[0, Frames.END_TRUE_POSITION_2D])
        np.testing.assert_equal((137.5, 127.5, 46), nodes[0, Frames.END_TRUE_POSITION_3D])

        self.assertEqual(17592186044420, nodes[0, Frames.SOURCE_MAC_ADDRESS])
        self.assertEqual(281474976710655, nodes[0, Frames.DESTINATION_MAC_ADDRESS])

        self.assertEqual(1, nodes[0, Frames.SEQUENCE_NUMBER])

        # Check load_csv() faults
        with self.assertRaises(ValueError):
            Frames.load_csv(None)

        with self.assertRaises(OSError):
            Frames.load_csv("nonexisting file")


if __name__ == '__main__':
    unittest.main()
