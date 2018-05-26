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

import numpy as np
import pandas as pd


class Array:
    Type = namedtuple('Type', ('columns', 'dtypes'))

    @staticmethod
    def prepare_frames_type(additional_columns=None):
        columns = ['node_mac_address',
                   'direction',
                   'begin_clock_timestamp',
                   'begin_simulation_timestamp',
                   'begin_true_position_x',
                   'begin_true_position_y',
                   'begin_true_position_z',
                   'end_clock_timestamp',
                   'end_simulation_timestamp',
                   'end_true_position_x',
                   'end_true_position_y',
                   'end_true_position_z',
                   'source_mac_address',
                   'destination_mac_address',
                   'sequence_number']

        dtypes = {'node_mac_address': np.int64,
                  'direction': np.unicode_,
                  'begin_clock_timestamp': np.int64,
                  'begin_simulation_timestamp': np.int64,
                  'begin_true_position_x': np.float64,
                  'begin_true_position_y': np.float64,
                  'begin_true_position_z': np.float64,
                  'end_clock_timestamp': np.int64,
                  'end_simulation_timestamp': np.int64,
                  'end_true_position_x': np.float64,
                  'end_true_position_y': np.float64,
                  'end_true_position_z': np.float64,
                  'source_mac_address': np.int64,
                  'destination_mac_address': np.int64,
                  'sequence_number': np.int64}

        return Array.__build_type(columns, dtypes, additional_columns)

    @staticmethod
    def prepare_nodes_type(additional_columns=None):
        columns = ['mac_address',
                   'position_x',
                   'position_y',
                   'position_z']

        dtypes = {'mac_address': np.int64,
                  'position_x': np.float64,
                  'position_y': np.float64,
                  'position_z': np.float64}

        return Array.__build_type(columns, dtypes, additional_columns)

    @staticmethod
    def prepare_results_type(additional_columns=None):
        columns = ['position_dimensions',
                   'position_x',
                   'position_y',
                   'position_z',
                   'begin_true_position_x',
                   'begin_true_position_y',
                   'begin_true_position_z',
                   'end_true_position_x',
                   'end_true_position_y',
                   'end_true_position_z',
                   'mac_address']

        dtypes = {'position_dimensions': np.int8,
                   'position_x': np.float64,
                   'position_y': np.float64,
                   'position_z': np.float64,
                   'begin_true_position_x': np.float64,
                   'begin_true_position_y': np.float64,
                   'begin_true_position_z': np.float64,
                   'end_true_position_x': np.float64,
                   'end_true_position_y': np.float64,
                   'end_true_position_z': np.float64,
                   'mac_address': np.int64}

        return Array.__build_type(columns, dtypes, additional_columns)

    @staticmethod
    def load_csv(array_type, file_path):
        return pd.read_csv(file_path, names=array_type.columns, dtype=array_type.dtypes)

    @staticmethod
    def create(array_type):
        return pd.DataFrame(columns=array_type.columns)

    @staticmethod
    def __build_type(columns, dtypes, additional_columns):
        if additional_columns is not None:
            for additional_column, additional_dtype in additional_columns:
                columns.append(additional_column)
                dtypes[additional_column] = additional_dtype

        return Array.Type(columns, dtypes)
