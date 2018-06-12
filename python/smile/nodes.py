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

from smile.array import Array
from os.path import expanduser, abspath


class Nodes(Array):
    def __init__(self, *args):
        super(Nodes, self).__init__()
        self.column_names["mac_address"] = 0
        self.column_names["position_x"] = 1
        self.column_names["position_y"] = 2
        self.column_names["position_z"] = 3

        self.column_names["position_2d"] = (self.column_names["position_x"],
                                            self.column_names["position_y"])

        self.column_names["position_3d"] = (self.column_names["position_x"],
                                            self.column_names["position_y"],
                                            self.column_names["position_z"])

    @staticmethod
    def load_csv(file_path):
        if isinstance(file_path, str):
            file_path = abspath(expanduser(file_path))
        return Nodes(np.loadtxt(file_path, delimiter=',', ndmin=2))
