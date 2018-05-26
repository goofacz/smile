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

from collections import namedtuple

Results = namedtuple('Result', ('position_dimensions',
                                'position_x',
                                'position_y',
                                'position_z',
                                'begin_true_position_x',
                                'begin_true_position_y',
                                'begin_true_position_z',
                                'end_true_position_x',
                                'end_true_position_y',
                                'end_true_position_z',
                                'mac_address'))
