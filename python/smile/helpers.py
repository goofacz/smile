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


def mac_address_to_string(mac_address):
    """
    Converts MAC address from int to pretty string (e.g, XX-XX-XX-XX-XX-XX).

    Parameters
    ----------
    mac_address : int
        MAC address

    Returns
    -------
    str
        Pretty string with MAC address
    """
    return '-'.join(format(x, '02X') for x in int(mac_address).to_bytes(6, 'big'))
