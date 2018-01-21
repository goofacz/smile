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

import numpy


class Nodes:
    # Columns in original CSV file
    _MAC_ADDRESS = 0
    _X_COORDINATE = 1
    _Y_COORDINATE = 2
    _Z_COORDINATE = 3

    def __init__(self, file_path=None):
        if file_path is not None:
            self.mac_addresses = numpy.loadtxt(file_path, delimiter=',', usecols=(Nodes._MAC_ADDRESS,),
                                               ndmin=1)

            self.positions = numpy.loadtxt(file_path, delimiter=',',
                                           usecols=(Nodes._X_COORDINATE, Nodes._Y_COORDINATE, Nodes._Z_COORDINATE),
                                           ndmin=2)
        else:
            self.mac_addresses = None
            self.positions = None

    def __getitem__(self, condition):
        nodes = Nodes()
        nodes.mac_addresses = self.mac_addresses[condition]
        nodes.positions = self.positions[condition]
        return nodes
