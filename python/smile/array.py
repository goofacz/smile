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
    def __new__(cls, input_array):
        instance = np.asarray(input_array).view(cls)
        instance.column_names = {}
        return instance

    def __array_finalize__(self, instance):
        if instance is None:
            return
        self.column_names = getattr(instance, "column_names", None)

    def __getitem__(self, index):
        index = self._prepare_index(index)
        return super(Array, self).__getitem__(index)

    def __setitem__(self, index, value):
        index = self._prepare_index(index)
        return super(Array, self).__setitem__(index, value)

    def _prepare_index(self, index):
        if isinstance(index, str):
            index = (slice(None, None, None), self.column_names[index])
        elif type(index) in (list, tuple):
            if isinstance(index[0], str):
                raise IndexError("Rows cannot be indexed with string names")
            if isinstance(index[1], str):
                if index[1] not in self.column_names:
                    raise IndexError("Unknown column name: '{0}'".format(index[1]))
                index = (index[0], self.column_names[index[1]])

        return index
