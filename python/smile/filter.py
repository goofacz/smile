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


class Filter(object):
    def __init__(self, array):
        self.array = array

    def less_equal(self, column, value):
        self.array = self.array[np.less_equal(self.array[:, column], value)]
        return self

    def less(self, column, value):
        self.array = self.array[np.less(self.array[:, column], value)]
        return self

    def greater_equal(self, column, value):
        self.array = self.array[np.greater_equal(self.array[:, column], value)]
        return self

    def greater(self, column, value):
        self.array = self.array[np.greater(self.array[:, column], value)]
        return self

    def equal(self, column, value):
        self.array = self.array[np.equal(self.array[:, column], value)]
        return self

    def not_equal(self, column, value):
        self.array = self.array[np.not_equal(self.array[:, column], value)]
        return self

    def is_in(self, column, value):
        self.array = self.array[np.isin(self.array[:, column], value)]
        return self

    def is_not_in(self, column, value):
        self.array = self.array[np.isin(self.array[:, column], value, invert=True)]
        return self

    def finish(self):
        return self.array
