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

import numpy as np
from os.path import expanduser, abspath

from smile.array import Array


class Frames(Array):
    def __init__(self, *args):
        super(Frames, self).__init__()
        self.column_names["node_mac_address"] = 0
        self.column_names["direction"] = 1
        self.column_names["begin_clock_timestamp"] = 2
        self.column_names["begin_simulation_timestamp"] = 3
        self.column_names["begin_true_position_x"] = 4
        self.column_names["begin_true_position_y"] = 5
        self.column_names["begin_true_position_z"] = 6
        self.column_names["end_clock_timestamp"] = 7
        self.column_names["end_simulation_timestamp"] = 8
        self.column_names["end_true_position_x"] = 9
        self.column_names["end_true_position_y"] = 10
        self.column_names["end_true_position_z"] = 11
        self.column_names["source_mac_address"] = 12
        self.column_names["destination_mac_address"] = 13
        self.column_names["sequence_number"] = 14

        self.column_names["begin_true_position_2d"] = (self.column_names["begin_true_position_x"],
                                                       self.column_names["begin_true_position_y"])

        self.column_names["begin_true_position_3d"] = (self.column_names["begin_true_position_x"],
                                                       self.column_names["begin_true_position_y"],
                                                       self.column_names["begin_true_position_z"])

        self.column_names["end_true_position_2d"] = (self.column_names["end_true_position_x"],
                                                     self.column_names["end_true_position_y"])

        self.column_names["end_true_position_3d"] = (self.column_names["end_true_position_x"],
                                                     self.column_names["end_true_position_y"],
                                                     self.column_names["end_true_position_z"])

    @staticmethod
    def load_csv(file_path):
        """
        Loads frames from CSV file.

        :param file_path: Path to CSV file
        """
        converters = Frames._get_default_converters()
        if isinstance(file_path, str):
            file_path = abspath(expanduser(file_path))
        return Frames(np.loadtxt(file_path, delimiter=',', converters=converters, ndmin=2))

    @staticmethod
    def _get_default_converters():
        return {1: lambda value: hash(value)}
