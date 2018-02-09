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


class Array(np.ndarray):
    def __new__(cls, input_array, info=None):
        instance = np.asarray(input_array).view(cls)
        instance.column_names = {}
        return instance

    def __array_finalize__(self, instance):
        if instance is None:
            return

        self.column_names = getattr(instance, "column_names", None)

    def __getitem__(self, index):
        if isinstance(index, str):
            index = self.column_names[index]

        return super(Array, self).__getitem__(index)
