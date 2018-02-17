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
    content = "17592186044417,0.000000,40.000000,0.000000\n17592186044418,75.000000,90.000000,70.000000"

    def test_load_csv(self):
        nodes = Nodes.load_csv(StringIO(TestNodes.content))
        self.assertTrue(isinstance(nodes, Nodes))
        self.assertTupleEqual((2, 4), nodes.shape)

    def test_column_access(self):
        nodes = Nodes.load_csv(StringIO(TestNodes.content))

        # Check field access
        self.assertEqual(17592186044417, nodes[0, "mac_address"])
        self.assertEqual(17592186044418, nodes[1, "mac_address"])

        self.assertEqual(75, nodes[1, "position_x"])
        self.assertEqual(90, nodes[1, "position_y"])
        self.assertEqual(70, nodes[1, "position_z"])

        np.testing.assert_equal((75, 90), nodes[1, "position_2d"])
        np.testing.assert_equal((75, 90, 70), nodes[1, "position_3d"])


if __name__ == '__main__':
    unittest.main()
