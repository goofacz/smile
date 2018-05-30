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


class Result(object):
    def __init__(self):
        self.position_dimensions = None
        self.position_x = None
        self.position_y = None
        self.position_z = None
        self.begin_true_position_x = None
        self.begin_true_position_y = None
        self.begin_true_position_z = None
        self.end_true_position_x = None
        self.end_true_position_y = None
        self.end_true_position_z = None
        self.mac_address = None

    def to_tuple(self):
        result = (self.position_dimensions,
                  self.position_x,
                  self.position_y,
                  self.position_z,
                  self.begin_true_position_x,
                  self.begin_true_position_y,
                  self.begin_true_position_z,
                  self.end_true_position_x,
                  self.end_true_position_y,
                  self.end_true_position_z,
                  self.mac_address)

        if not all(element is not None for element in result):
            raise ValueError('Result tuple cannot contain None values')

        return result


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
    def create_array(rows):
        results = [row.to_tuple() for row in rows]
        results = Results(results)

        unique = np.unique(results['position_dimensions'])
        if unique.shape != (1,):
            raise ValueError('Array cannot store 2d and 3D positions at the same time')

        return results
