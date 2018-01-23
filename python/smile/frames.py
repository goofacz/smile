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


class Frames:
    # Columns in original CSV file
    DIRECTION = 0
    BEGIN_CLOCK_TIMESTAMP = 1
    BEGIN_SIMULATION_TIMESTAMP = 2
    BEGIN_TRUE_POSITION_X = 3
    BEGIN_TRUE_POSITION_Y = 4
    BEGIN_TRUE_POSITION_Z = 5
    END_CLOCK_TIMESTAMP = 6
    END_SIMULATION_TIMESTAMP = 7
    END_TRUE_POSITION_X = 8
    END_TRUE_POSITION_Y = 9
    END_TRUE_POSITION_Z = 10
    SOURCE_MAC_ADDRESS = 11
    DESTINATION_MAC_ADDRESS = 12
    SEQUENCE_NUMBER = 13

    BEGIN_TRUE_POSITION_2D = (BEGIN_TRUE_POSITION_X, BEGIN_TRUE_POSITION_Y)
    BEGIN_TRUE_POSITION_3D = (BEGIN_TRUE_POSITION_X, BEGIN_TRUE_POSITION_Y, BEGIN_TRUE_POSITION_Z)

    END_TRUE_POSITION_2D = (END_TRUE_POSITION_X, END_TRUE_POSITION_Y)
    END_TRUE_POSITION_3D = (END_TRUE_POSITION_X, END_TRUE_POSITION_Y, END_TRUE_POSITION_Z)

    @staticmethod
    def load_csv(file_path):
        """
        Loads frames from CSV file.

        :param file_path: Path to CSV file
        """
        converters = {Frames.DIRECTION: lambda value: hash(value)}
        return np.loadtxt(file_path, delimiter=',', converters=converters, ndmin=2)