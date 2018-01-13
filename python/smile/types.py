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

from sympy.physics.units import picoseconds, convert_to
from numpy import int64


class MACAddress:
    """
    Simple class handling MAC addresses.
    """

    def __init__(self, address):
        if isinstance(address, int):
            self.address = address
            return

        if isinstance(address, int64):
            self.address = int(address)
            return

        raise TypeError("MACAddress accepts only int and numpy.int64")

    def __str__(self):
        return '-'.join(format(x, '02X') for x in self.address.to_bytes(6, 'big'))

    def __int__(self):
        return self.address

    def __eq__(self, other):
        return self.address == other.address


class Timestamp:
    """
    Simple class handling timestamps.
    """

    def __init__(self, timestamp, unit=picoseconds):
        if not isinstance(timestamp, int):
            raise TypeError("Timestamp accepts only address")
        self.timestamp = int(timestamp) * unit

    def __eq__(self, other):
        return self.__dict__ == other.__dict__

    def __str__(self):
        return str(self.timestamp)

    def convert_to(self, unit):
        return convert_to(self.timestamp, unit)
