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
    """
    This class holds information about frames transmitted or received by particular radio node.

    Attributes:

    directions : numpy.ndarray (m x 1)
        Tells whether frame was transmitted ('TX') or received ('RX'). Array holds hash of 'TX' or 'RX' string.
    begin_timestamps : numpy.ndarray (m x 2)
        Timestamp of beginning of frame transmission/reception according to local clock and global simulation clock
        respectively.
    begin_positions : numpy.ndarray (m x 3)
        True position of node when frame transmission/reception started expressed as (X, Y, Z) coordinates.
    end_timestamps : numpy.ndarray (m x 2)
        Timestamp of end of frame transmission/reception according to local clock and global simulation clock
        respectively.
    end_positions : numpy.ndarray (m x 3)
        True position of node when frame transmission/reception finished expressed as (X, Y, Z) coordinates.
    mac_addresses : numpy.ndarray (m x 2)
        Frames source and destination MAC address respectively.
    sequence_numbers : numpy.ndarray (m x 1)
        Frames' sequence numbers.
    """
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

    def __init__(self, file_path=None):
        """
        Constructs and loads frames data from CSV file.

        :param file_path: Path to CSV file with frames log.
        """
        if file_path is not None:
            self.directions = loadtxt(file_path, delimiter=',', dtype='U2', usecols=Frames._DIRECTION, ndmin=1)

            self.begin_timestamps = loadtxt(file_path, delimiter=',',
                                            usecols=(Frames._BEGIN_CLOCK_TIMESTAMP, Frames._BEGIN_SIMULATION_TIMESTAMP),
                                            ndmin=2)

            self.begin_positions = loadtxt(file_path, delimiter=',',
                                           usecols=(Frames._BEGIN_TRUE_POSITION_X, Frames._BEGIN_TRUE_POSITION_Y,
                                                    Frames._BEGIN_TRUE_POSITION_Z), ndmin=2)

            self.end_timestamps = loadtxt(file_path, delimiter=',',
                                          usecols=(Frames._END_CLOCK_TIMESTAMP, Frames._END_SIMULATION_TIMESTAMP),
                                          ndmin=2)

            self.end_positions = loadtxt(file_path, delimiter=',',
                                         usecols=(Frames._END_TRUE_POSITION_X, Frames._END_TRUE_POSITION_Y,
                                                  Frames._END_TRUE_POSITION_Z), ndmin=2)

            self.mac_addresses = loadtxt(file_path, delimiter=',',
                                         usecols=(Frames._SOURCE_MAC_ADDRESS, Frames._DESTINATION_MAC_ADDRESS), ndmin=2)

            self.sequence_numbers = loadtxt(file_path, delimiter=',', usecols=Frames._SEQUENCE_NUMBER,
                                            ndmin=1)
        else:
            self.directions = None
            self.begin_timestamps = None
            self.begin_positions = None
            self.end_timestamps = None
            self.end_positions = None
            self.mac_addresses = None
            self.sequence_numbers = None

    def __getitem__(self, condition):
        frames = Frames()
        frames.directions = self.directions[condition]
        frames.begin_timestamps = self.begin_timestamps[condition]
        frames.begin_positions = self.begin_positions[condition]
        frames.end_timestamps = self.end_timestamps[condition]
        frames.end_positions = self.end_positions[condition]
        frames.mac_addresses = self.mac_addresses[condition]
        frames.sequence_numbers = self.sequence_numbers[condition]
        return frames
