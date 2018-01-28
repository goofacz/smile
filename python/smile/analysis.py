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
import matplotlib.pyplot as plt
from smile.results import Results


def _determine_dimensions(results):
    unique_dimensions = np.unique(results[:, Results.POSITION_DIMENSIONS])
    if len(unique_dimensions) is not 1:
        raise ValueError('Cannot process results with different position dimensions (2D and 3D)')

    if results[0, Results.POSITION_DIMENSIONS] == 2:
        return Results.POSITION_2D, Results.BEGIN_TRUE_POSITION_2D,  Results.END_TRUE_POSITION_2D
    else:
        return Results.POSITION_3D, Results.BEGIN_TRUE_POSITION_3D,  Results.END_TRUE_POSITION_3D


def absolute_position_error_histogram(results):
    position_coordinates, begin_position_coordinates, end_position_coordinates = _determine_dimensions(results)
    # Mobile node cloud move during localization procedure
    true_position = (results[:, begin_position_coordinates] + results[:, end_position_coordinates]) / 2
    position_errors = np.abs(np.linalg.norm(true_position - results[:, position_coordinates], axis=1))

    plt.hist(position_errors)
    plt.title('Histogram of absolute error values')
    plt.xlabel('Position error [m]')
    plt.ylabel('Number of localization results')
    plt.grid(True)
    plt.show()


def absolute_position_error_surface(results):
    position_coordinates, begin_position_coordinates, end_position_coordinates = _determine_dimensions(results)

    # We want to have single result per mobile node
    unique_mac_addresses = np.unique(results[:, Results.MAC_ADDRESS])
    unique_results = Results.create_array(len(unique_mac_addresses))

    for i in range(0, len(unique_mac_addresses)):
        mac_address = unique_mac_addresses[i]
        tmp_results = results[results[:, Results.MAC_ADDRESS] == mac_address]

        unique_results[i, :] = tmp_results[0, :]
        # TODO Implement more ways of "squeezing" multiple results into one
        unique_results[i, begin_position_coordinates] = np.mean(tmp_results[:, begin_position_coordinates], axis=0)
        unique_results[i, end_position_coordinates] = np.mean(tmp_results[:, end_position_coordinates], axis=0)
        unique_results[i, position_coordinates] = np.mean(tmp_results[:, position_coordinates], axis=0)

    # Mobile node cloud move during localization procedure
    true_positions = (unique_results[:, begin_position_coordinates] + unique_results[:, end_position_coordinates]) / 2
    position_errors = np.abs(np.linalg.norm(true_positions - unique_results[:, position_coordinates], axis=1))

    x, y = np.meshgrid(np.unique(true_positions[:, 0]), np.unique(true_positions[:, 1]), indexing='xy')
    z = np.zeros(x.shape)

    # Assign position errors (z) to correct node coordinates (x, y)
    for i in range(0, len(true_positions)):
        tmp_true_position = true_positions[i, :]
        tmp_position_error = position_errors[i]

        tmp_z_indices = np.where(np.logical_and(x == tmp_true_position[0], y == tmp_true_position[1]))
        z[tmp_z_indices] = tmp_position_error

    plt.pcolormesh(x, y, z)
    y_min, y_max = plt.ylim()
    plt.ylim(y_max, y_min)
    axis = plt.gca()
    axis.xaxis.set_ticks_position('top')
    axis.xaxis.set_label_position('top')
    plt.colorbar().set_label('Error value [m]')
    plt.grid()
    plt.title('Map of absolute position errors', y=-0.1)
    plt.xlabel('X [m]')
    plt.ylabel('Y [m]')
    plt.show()
