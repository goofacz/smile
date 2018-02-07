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

from enum import Enum

import numpy as np


class Filter(object):
    class Check(object):
        class Condition(Enum):
            LESS_EQUAL = 1
            LESS = 2
            GREATER_EQUAL = 3
            GREATER = 4
            EQUAL = 5
            IS_IN = 6

        operations = {
            Condition.EQUAL: (lambda array, column, value: array[:, int(column)] == value),
            Condition.GREATER: (lambda array, column, value: array[:, int(column)] > value),
            Condition.GREATER_EQUAL: (lambda array, column, value: array[:, int(column)] >= value),
            Condition.IS_IN: (lambda array, column, value: np.isin(array[:, int(column)], value)),
            Condition.LESS: (lambda array, column, value: array[:, int(column)] < value),
            Condition.LESS_EQUAL: (lambda array, column, value: array[:, int(column)] <= value)
        }

        def __init__(self, condition, column, value):
            self.condition = condition
            self.column = column
            self.value = value

        def execute(self, array):
            indices = Filter.Check.operations[self.condition](array, self.column, self.value)
            return array[indices, :]

    def __init__(self):
        self._checks = []

    def less_equal(self, column, value):
        self._checks.append(Filter.Check(Filter.Check.Condition.LESS_EQUAL, column, value))
        return self

    def less(self, column, value):
        self._checks.append(Filter.Check(Filter.Check.Condition.LESS, column, value))
        return self

    def greater_equal(self, column, value):
        self._checks.append(Filter.Check(Filter.Check.Condition.GREATER_EQUAL, column, value))
        return self

    def greater(self, column, value):
        self._checks.append(Filter.Check(Filter.Check.Condition.GREATER, column, value))
        return self

    def equal(self, column, value):
        self._checks.append(Filter.Check(Filter.Check.Condition.EQUAL, column, value))
        return self

    def is_in(self, column, value):
        self._checks.append(Filter.Check(Filter.Check.Condition.IS_IN, column, value))
        return self

    def execute(self, array):
        result = array
        for check in self._checks:
            result = check.execute(result)

        return result
