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


class Nodes:
    MAC_ADDRESS = 0
    POSITION_X = 1
    POSITION_Y = 2
    POSITION_Z = 3

    POSITION_2D = (POSITION_X, POSITION_Y)
    POSITION_3D = (POSITION_X, POSITION_Y, POSITION_Z)

    @staticmethod
    def load_csv(file_path):
        return np.loadtxt(file_path, delimiter=',', ndmin=2)
