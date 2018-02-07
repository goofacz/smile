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


class Condition(Enum):
    LESS_EQUAL = 1
    LESS = 2
    GREATER_EQUAL = 3
    GREATER = 4
    EQUAL = 5
    IN = 6


class Filter(object):
    class Check(object):
        operations = {
            Condition.EQUAL: (lambda array, column, value: array[:, int(column)] == value),
            Condition.GREATER: (lambda array, column, value: array[:, int(column)] > value),
            Condition.GREATER_EQUAL: (lambda array, column, value: array[:, int(column)] >= value),
            Condition.IN: (lambda array, column, value: np.isin(array[:, int(column)], value)),
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

    def append(self, condition, column, value):
        self._checks.append(Filter.Check(condition, column, value))

    def execute(self, array):
        result = array
        for check in self._checks:
            result = check.execute(result)

        return result
