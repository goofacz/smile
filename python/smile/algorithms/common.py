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


def sort_distances(coordinates, distances):
    if len(distances.shape) != 1:
        raise AttributeError("distances has to be a vector")
    if distances.shape[0] != coordinates.shape[0]:
        raise AttributeError("distances and coordinates must have the same number of entries")
    if len(coordinates.shape) == 1 or coordinates.shape[1] not in (2, 3):
        raise AttributeError("coordinates ,ust have two or thee columns")

    indices = np.argsort(distances)
    return coordinates[indices, :], distances[indices]
