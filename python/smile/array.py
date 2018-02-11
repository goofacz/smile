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

    def _process_vector_index(self, index):
        if isinstance(index, str):
            if index not in self.column_names:
                raise IndexError("Unknown column name: '{0}'".format(index))
            index = self.column_names[index]
            return index, True

        if index == slice(None, None, None):
            return index, False

        return index, True

    def _process_array_index(self, index):
        if isinstance(index, str):
            if index not in self.column_names:
                raise IndexError("Unknown column name: '{0}'".format(index))
            index = (slice(None, None, None), self.column_names[index])
            return index, True

        if type(index) in (list, tuple):
            return_as_ndarray = False
            if len(index) == 1:
                index = index[0]
            elif len(index) == 2:
                if isinstance(index[0], str):
                    raise IndexError("Rows cannot be indexed with string names")
                elif type(index[0]) in (list, tuple) and type(index[0][0]) is np.ndarray:
                    index = (index[0][0], index[1])

                if isinstance(index[1], str):
                    if index[1] not in self.column_names:
                        raise IndexError("Unknown column name: '{0}'".format(index[1]))
                    index = (index[0], self.column_names[index[1]])

                if index[1] != slice(None, None, None):
                    return_as_ndarray = True
            else:
                raise IndexError("Sequence of {0} cannot be used for indexing")

            return index, return_as_ndarray

        if isinstance(index, np.ndarray):
            return index, False

        if isinstance(index, int):
            index = (index, slice(None, None, None))
            return index, False

        raise IndexError('Invalid index')

    def _process_index(self, index):
        if len(self.shape) == 1:
            return self._process_vector_index(index)
        else:
            return self._process_array_index(index)