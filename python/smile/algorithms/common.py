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
    if len(anchor_coordinates.shape) != 2:
        raise ValueError("Anchors coordinates has to be 2D array")
    if anchor_coordinates.shape[0] == 0:
        raise ValueError("At least one anchor has to be provided")
    if anchor_coordinates.shape[1] not in (2, 3):
        raise ValueError("Anchors' coordinates has to be either in 2D or 3D")
    if grid_size <= 0:
        raise ValueError("Grid size cannot be <= 0")
    if not (0 < grid_gap < grid_size):
        raise ValueError("Grid gap hast bo in range (0, grid size)")

    for mobile_coordinates in itertools.product(np.arange(0, grid_size, grid_gap), repeat=2):
        distances = __generate_distances(anchor_coordinates, mobile_coordinates)
        yield np.asanyarray(mobile_coordinates), distances


def generate_tdoa_measurements(anchor_coordinates, grid_size, grid_gap):
    for mobile_coordinates, distances in generate_tof_measurements(anchor_coordinates, grid_size, grid_gap):
        yield mobile_coordinates, distances - min(distances)


def does_area_contain_position(position, top_left, bottom_right, rtol=1.e-5, atol=1.e-8):
    if position[0] < top_left[0] and (not np.isclose(top_left[0], position[0], rtol=rtol, atol=atol)):
        return False
    if position[0] > bottom_right[0] and (not np.isclose(bottom_right[0], position[0], rtol=rtol, atol=atol)):
        return False

    if position[1] > top_left[1] and (not np.isclose(top_left[1], position[1], rtol=rtol, atol=atol)):
        return False
    if position[1] < bottom_right[1] and (not np.isclose(bottom_right[1], position[1], rtol=rtol, atol=atol)):
        return False

    return True
