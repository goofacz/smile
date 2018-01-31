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
from smile.nodes import Nodes


class TestNodes(unittest.TestCase):
    def test_load_csv(self):
        content = StringIO("17592186044417,0.000000,40.000000,0.000000\n17592186044418,75.000000,90.000000,70.000000")

        # Check whether data is loaded into correct array
        nodes = Nodes.load_csv(content)
        self.assertTrue(isinstance(nodes, np.ndarray))
        self.assertTupleEqual((2, 4), nodes.shape)

        # Check field access
        self.assertEqual(17592186044417, nodes[0, Nodes.MAC_ADDRESS])
        self.assertEqual(17592186044418, nodes[1, Nodes.MAC_ADDRESS])

        self.assertEqual(75, nodes[1, Nodes.POSITION_X])
        self.assertEqual(90, nodes[1, Nodes.POSITION_Y])
        self.assertEqual(70, nodes[1, Nodes.POSITION_Z])

        np.testing.assert_equal((75, 90), nodes[1, Nodes.POSITION_2D])
        np.testing.assert_equal((75, 90, 70), nodes[1, Nodes.POSITION_3D])

        # Check load_csv() faults
        with self.assertRaises(ValueError):
            Nodes.load_csv(None)

        with self.assertRaises(OSError):
            Nodes.load_csv("nonexisting file")


if __name__ == '__main__':
    unittest.main()
