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

import matplotlib.pyplot as plt
import numpy as np


def absolute_position_error_histogram(results, return_intermediate_results=False):
    position_coordinates, begin_position_coordinates, end_position_coordinates = results.determine_dimensions()
    # Mobile node cloud move during localization procedure
    true_positions = (results[:, begin_position_coordinates] + results[:, end_position_coordinates]) / 2
    position_errors = np.abs(np.linalg.norm(true_positions - results[:, position_coordinates], axis=1))

    plt.hist(position_errors)
    plt.title('Histogram of absolute error values')
    plt.xlabel('Position error [m]')
    plt.ylabel('Number of localization results')
    plt.grid(True)
    plt.show()

    if return_intermediate_results:
        return true_positions, position_errors


def absolute_position_error_surface(results, return_intermediate_results=False):
    position_coordinates, begin_position_coordinates, end_position_coordinates = results.determine_dimensions()

    # Mobile node cloud move during localization procedure
    true_positions = (results[:, begin_position_coordinates] + results[:, end_position_coordinates]) / 2
    position_errors = np.abs(np.linalg.norm(true_positions - results[:, position_coordinates], axis=1))

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

    if return_intermediate_results:
        return true_positions, position_errors


def plot_absolute_position_error_cdf(results):
    position_coordinates, begin_position_coordinates, end_position_coordinates = results.determine_dimensions()

    true_positions = (results[:, begin_position_coordinates] + results[:, end_position_coordinates]) / 2
    position_errors = np.abs(np.linalg.norm(true_positions - results[:, position_coordinates], axis=1))
    position_errors = np.sort(position_errors)
    n = np.array(range(position_errors.size)) / np.float(position_errors.size)

    plt.plot(position_errors, n)
    plt.grid()
    plt.title('CDF of absolute position error')
    plt.xlabel('Error [m]')
    plt.show()
