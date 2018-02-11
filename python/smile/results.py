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


class Results(Array):
    def __init__(self, *args):
        super(Results, self).__init__()
        self.column_names["position_dimensions"] = 0
        self.column_names["position_x"] = 1
        self.column_names["position_y"] = 2
        self.column_names["position_z"] = 3
        self.column_names["begin_true_position_x"] = 4
        self.column_names["begin_true_position_y"] = 5
        self.column_names["begin_true_position_z"] = 6
        self.column_names["end_true_position_x"] = 7
        self.column_names["end_true_position_y"] = 8
        self.column_names["end_true_position_z"] = 9
        self.column_names["mac_address"] = 10

        self.column_names["position_2d"] = (self.column_names["position_x"],
                                            self.column_names["position_y"])

        self.column_names["position_3d"] = (self.column_names["position_x"],
                                            self.column_names["position_y"],
                                            self.column_names["position_z"])

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
    def create_array(rows, position_dimensions=2, mac_address=None):
        assert(position_dimensions in (2, 3))
        results = Results(np.zeros((rows, 11)))
        results["position_dimensions"] = position_dimensions
        if mac_address is not None:
            results["mac_address"] = mac_address
        return results
