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
import matplotlib.pyplot as pyplot
from smile.helpers import PositionDimensions


def absolute_position_error(result):
    if result.position_dimensions is PositionDimensions.TWO_D:
        coordinates = (0, 1)
    elif result.position_dimensions is PositionDimensions.THREE_D:
        coordinates = (0, 1, 2)

    true_position = (result.begin_true_position[:, coordinates] + result.end_true_position[:, coordinates]) / 2
    position_error = numpy.abs(numpy.linalg.norm(true_position - result.position[:, coordinates], axis=1))

    pyplot.hist(position_error)
    pyplot.title('Histogram of absolute error values')
    pyplot.xlabel('Position error [m]')
    pyplot.ylabel('Number of localization results')
    pyplot.grid(True)
    pyplot.show()