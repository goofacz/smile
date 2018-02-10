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

    def __array_wrap__(self, out_arr, context=None):
        return np.ndarray.__array_wrap__(self.view(np.ndarray), out_arr, context)

    def __getitem__(self, index):
        index, return_as_ndarray = self._process_index(index)
        result = super(Array, self).__getitem__(index)
        if return_as_ndarray:
            return result.view(np.ndarray)
        else:
            return result

    def __setitem__(self, index, value):
        index, _ = self._process_index(index)
        return super(Array, self).__setitem__(index, value)

    def _process_index(self, index):
        return_as_ndarray = False
        if isinstance(index, str):
            index = (slice(None, None, None), self.column_names[index])
            return_as_ndarray = True
        elif type(index) in (list, tuple):
            if isinstance(index[0], str):
                raise IndexError("Rows cannot be indexed with string names")
            if isinstance(index[1], str):
                if index[1] not in self.column_names:
                    raise IndexError("Unknown column name: '{0}'".format(index[1]))
                index = (index[0], self.column_names[index[1]])

            if index[1] != slice(None, None, None):
                return_as_ndarray = True

        return index, return_as_ndarray
