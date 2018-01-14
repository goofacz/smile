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

from numpy import loadtxt


class Frames:
    # Columns in original CSV file
    _DIRECTION = 0
    _BEGIN_CLOCK_TIMESTAMP = 1
    _BEGIN_SIMULATION_TIMESTAMP = 2
    _BEGIN_TRUE_POSITION_X = 3
    _BEGIN_TRUE_POSITION_Y = 4
    _BEGIN_TRUE_POSITION_Z = 5
    _END_CLOCK_TIMESTAMP = 6
    _END_SIMULATION_TIMESTAMP = 7
    _END_TRUE_POSITION_X = 8
    _END_TRUE_POSITION_Y = 9
    _END_TRUE_POSITION_Z = 10
    _SOURCE_MAC_ADDRESS = 11
    _DESTINATION_MAC_ADDRESS = 12
    _SEQUENCE_NUMBER = 13

    def __init__(self, file_path):
        self.directions = loadtxt(file_path, delimiter=',', dtype='U2', usecols=Frames._DIRECTION, ndmin=1)

        self.begin_timestamps = loadtxt(file_path, delimiter=',', dtype='uint64',
                                        usecols=(Frames._BEGIN_CLOCK_TIMESTAMP, Frames._BEGIN_SIMULATION_TIMESTAMP),
                                        ndmin=2)

        self.begin_positions = loadtxt(file_path, delimiter=',', dtype='double',
                                       usecols=(Frames._BEGIN_TRUE_POSITION_X, Frames._BEGIN_TRUE_POSITION_Y,
                                                Frames._BEGIN_TRUE_POSITION_Z), ndmin=2)

        self.end_timestamps = loadtxt(file_path, delimiter=',', dtype='uint64',
                                      usecols=(Frames._END_CLOCK_TIMESTAMP, Frames._END_SIMULATION_TIMESTAMP), ndmin=2)

        self.end_positions = loadtxt(file_path, delimiter=',', dtype='double',
                                     usecols=(Frames._END_TRUE_POSITION_X, Frames._END_TRUE_POSITION_Y,
                                              Frames._END_TRUE_POSITION_Z), ndmin=2)

        self.mac_addresses = loadtxt(file_path, delimiter=',', dtype='uint64',
                                     usecols=(Frames._SOURCE_MAC_ADDRESS, Frames._DESTINATION_MAC_ADDRESS), ndmin=2)

        self.sequence_numbers = loadtxt(file_path, delimiter=',', dtype='uint64', usecols=Frames._SEQUENCE_NUMBER,
                                        ndmin=1)
