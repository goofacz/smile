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

import itertools
import numpy as np
import scipy.constants as sc


def __compute_distance(anchor_coordinates, mobile_coordinates):
    return np.abs(np.linalg.norm(mobile_coordinates - anchor_coordinates))


def __generate_distances(anchors_coordinates, mobile_coordinates):
    return np.apply_along_axis(__compute_distance, 1, anchors_coordinates, mobile_coordinates)


def sort_measurements(coordinates, distances):
    if len(distances.shape) != 1:
        raise ValueError("distances has to be a vector")
    if distances.shape[0] != coordinates.shape[0]:
        raise ValueError("distances and coordinates must have the same number of entries")
    if len(coordinates.shape) == 1 or coordinates.shape[1] not in (2, 3):
        raise ValueError("coordinates ,ust have two or thee columns")

    indices = np.argsort(distances)
    return coordinates[indices, :], distances[indices]


def generate_tof_measurements(anchor_coordinates, grid_size, grid_gap):
    assert (sc.unit('speed of light in vacuum') == 'm s^-1')
    c = sc.value('speed of light in vacuum')
    c = c * 1e-12  # m/s -> m/ps

    for mobile_coordinates in itertools.product(range(0, grid_size + 1, grid_gap), repeat=2):
        measurements = __generate_distances(anchor_coordinates, mobile_coordinates)
        yield np.asanyarray(mobile_coordinates), measurements * c


def generate_tdoa_measurements(anchor_coordinates, grid_size, grid_gap):
    for mobile_coordinates, measurements in generate_tof_measurements(anchor_coordinates, grid_size, grid_gap):
        yield mobile_coordinates, measurements - min(measurements)

