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
import pandas as pd


class Frames:
    @classmethod
    def load_csv(cls, file_path):
        columns = cls.__get_columns()
        return pd.read_csv(file_path, names=columns.keys(), dtype=columns)

    @classmethod
    def __get_columns(cls):
        return {'node_mac_address': np.int64,
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
