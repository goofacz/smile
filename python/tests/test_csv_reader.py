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

from smile.csv_reader import *


class TestLoaders(unittest.TestCase):
    def test_load_anchors(self):
        anchors = load_anchors('ss_twr_anchor_nodes.csv')
        self.assertEqual(3, anchors.size)
        self.assertEqual(244837814046721, anchors[0]['mac_address'])
        self.assertEqual(200, anchors[0]['position_x'])
        self.assertEqual(100, anchors[0]['position_y'])
        self.assertEqual(0, anchors[0]['position_z'])

    def test_load_mobiles(self):
        mobiles = load_mobiles('ss_twr_mobile_nodes.csv')
        self.assertEqual(1, mobiles.size)
        self.assertEqual(244837814046736, mobiles[0]['mac_address'])
        self.assertEqual(200, mobiles[0]['position_x'])
        self.assertEqual(200, mobiles[0]['position_y'])
        self.assertEqual(0, mobiles[0]['position_z'])

    def test_load_frames(self):
        frames = load_frames('ss_twr_frames.csv')
        self.assertEqual(7, len(frames))
        self.assertEqual('TX', frames[0]['direction'])
        self.assertEqual(0, frames[0]['begin_clock_timestamp'])
        self.assertEqual(0, frames[0]['begin_simulation_timestamp'])
        self.assertEqual(200, frames[0]['begin_true_position_x'])
        self.assertEqual(200, frames[0]['begin_true_position_y'])
        self.assertEqual(0, frames[0]['begin_true_position_z'])
        self.assertEqual(10000000000, frames[0]['end_clock_timestamp'])
        self.assertEqual(10000000000, frames[0]['end_simulation_timestamp'])
        self.assertEqual(200, frames[0]['end_true_position_x'])
        self.assertEqual(200, frames[0]['end_true_position_y'])
        self.assertEqual(0, frames[0]['end_true_position_z'])
        self.assertEqual(244837814046736, frames[0]['source_mac_address'])
        self.assertEqual(244837814046721, frames[0]['destination_mac_address'])
        self.assertEqual(1, frames[0]['sequence_number'])

