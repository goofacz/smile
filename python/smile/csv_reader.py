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


def load_anchors(file_name, extended_column_dtype_names=(), extended_column_dtype_formats=()):
    """
    Loads anchor nodes from CSV file and returns them as numpy.array.
    If file contains additional columns they can be handled by setting extended_column_dtype_names and
    extended_column_dtype_formats.
    """
    columns = numpy.dtype({'names': ('mac_address', 'position_x', 'position_y', 'position_z',
                                     *extended_column_dtype_names),
                           'formats': (int, float, float, float, *extended_column_dtype_formats)})
    return numpy.loadtxt(file_name, dtype=columns, delimiter=',', ndmin=1)


def load_mobiles(file_name, extended_column_dtype_names=(), extended_column_dtype_formats=()):
    """
    Loads mobile nodes from CSV file and returns them as numpy.array.
    If file contains additional columns they can be handled by setting extended_column_dtype_names and
    extended_column_dtype_formats.
    """
    columns = numpy.dtype({'names': ('mac_address', 'position_x', 'position_y', 'position_z',
                                     *extended_column_dtype_names),
                           'formats': (int, float, float, float, *extended_column_dtype_formats)})
    return numpy.loadtxt(file_name, dtype=columns, delimiter=',', ndmin=1)


def load_frames(file_name, extended_column_dtype_names=(), extended_column_dtype_formats=()):
    """
    Loads frames from CSV file and returns them as numpy.array.
    If file contains additional columns they can be handled by setting extended_column_dtype_names and
    extended_column_dtype_formats.
    """
    columns = numpy.dtype({'names': ('direction',
                                     'begin_clock_timestamp', 'begin_simulation_timestamp',
                                     'begin_true_position_x', 'begin_true_position_y', 'begin_true_position_z',
                                     'end_clock_timestamp', 'end_simulation_timestamp',
                                     'end_true_position_x', 'end_true_position_y', 'end_true_position_z',
                                     'source_mac_address', 'destination_mac_address',
                                     'sequence_number', *extended_column_dtype_names),
                           'formats': ('U2',
                                       int, int,
                                       float, float, float,
                                       int, int,
                                       float, float, float,
                                       int, int,
                                       int, *extended_column_dtype_formats)})
    return numpy.loadtxt(file_name, dtype=columns, delimiter=',', ndmin=1)
