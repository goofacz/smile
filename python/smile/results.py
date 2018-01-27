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


class Results:
    POSITION_DIMENSIONS = 0
    POSITION_X = 1
    POSITION_Y = 2
    POSITION_Z = 3
    BEGIN_TRUE_POSITION_X = 4
    BEGIN_TRUE_POSITION_Y = 5
    BEGIN_TRUE_POSITION_Z = 6
    END_TRUE_POSITION_X = 7
    END_TRUE_POSITION_Y = 8
    END_TRUE_POSITION_Z = 9
    MAC_ADDRESS = 10

    POSITION_2D = (POSITION_X, POSITION_Y)
    POSITION_3D = (POSITION_X, POSITION_Y, POSITION_Z)

    BEGIN_TRUE_POSITION_2D = (BEGIN_TRUE_POSITION_X, BEGIN_TRUE_POSITION_Y)
    BEGIN_TRUE_POSITION_3D = (BEGIN_TRUE_POSITION_X, BEGIN_TRUE_POSITION_Y, BEGIN_TRUE_POSITION_Z)

    END_TRUE_POSITION_2D = (END_TRUE_POSITION_X, END_TRUE_POSITION_Y)
    END_TRUE_POSITION_3D = (END_TRUE_POSITION_X, END_TRUE_POSITION_Y, END_TRUE_POSITION_Z)

    @staticmethod
    def create_array(rows, position_dimensions):
        array = np.zeros((rows, 11))
        assert(position_dimensions in (2, 3))
        array[:,0] = position_dimensions
        return array
