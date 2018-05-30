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
import numpy.linalg as npla


def plot_absolute_position_error_histogram(results, return_intermediate_results=False):
    # FIXME
    position_coordinates = 'position_2d' #['position_x', 'position_y', 'position_z']
    begin_position_coordinates = 'begin_true_position_2d' #['begin_true_position_x', 'begin_true_position_y', 'begin_true_position_z']
    end_position_coordinates = 'end_true_position_2d' #['end_true_position_x', 'end_true_position_y', 'end_true_position_z']

    # Mobile node cloud move during localization procedure
    true_positions = (results[:, begin_position_coordinates] + results[:, end_position_coordinates]) / 2
    position_errors = npla.norm(true_positions - results[:, position_coordinates], axis=1)

    # Process data (remove NaNs)
    nans_number = np.count_nonzero(np.isnan(position_errors))
    position_errors = position_errors[np.isfinite(position_errors)]

    plt.hist(position_errors)
    plt.title('Histogram of absolute error values')
    plt.xlabel('Position error [m]')
    plt.ylabel('Number of localization results')
    plt.grid(True)
    plt.show()

    if return_intermediate_results:
        return true_positions, position_errors, nans_number


def plot_absolute_position_error_surface(results, anchors=None, return_intermediate_results=False):
    # FIXME
    position_coordinates = 'position_2d' #['position_x', 'position_y', 'position_z']
    begin_position_coordinates = 'begin_true_position_2d' #['begin_true_position_x', 'begin_true_position_y', 'begin_true_position_z']
    end_position_coordinates = 'end_true_position_2d' #['end_true_position_x', 'end_true_position_y', 'end_true_position_z']

    # Mobile node cloud move during localization procedure
    true_positions = (results[:, begin_position_coordinates] + results[:, end_position_coordinates]) / 2
    position_errors = npla.norm(true_positions - results[:, position_coordinates], axis=1)

    x, y = np.meshgrid(np.unique(true_positions[:, 0]), np.unique(true_positions[:, 1]), indexing='xy')
    z = np.zeros(x.shape)

    # Assign position errors (z) to correct node coordinates (x, y)
    for i in range(0, len(true_positions)):
        tmp_true_position = true_positions[i, :]
        tmp_position_error = position_errors[i]

        tmp_z_indices = np.where(np.logical_and(x == tmp_true_position[0], y == tmp_true_position[1]))
        z[tmp_z_indices] = tmp_position_error

    # Plot errors
    plt.pcolormesh(x, y, z)

    x = x.flatten().tolist()
    y = y.flatten().tolist()

    # Plot anchors' positions
    if anchors is not None:
        for i in range(anchors.shape[0]):
            anchor_x = anchors[i, "position_x"]
            anchor_y = anchors[i, "position_y"]
            plt.plot(anchor_x, anchor_y, "ro", markersize=10)

            x.append(anchor_x)
            y.append(anchor_y)

    # Set canvas limits
    x_min = min(x) - 2.5
    x_max = max(x) + 2.5
    y_min = min(y) - 2.5
    y_max = max(y) + 2.5
    plt.xlim(x_min, x_max)
    plt.ylim(y_max, y_min)

    axis = plt.gca()
    axis.xaxis.set_ticks_position('top')
    axis.xaxis.set_label_position('top')
    axis.minorticks_on()
    plt.colorbar().set_label('Error value [m]')
    plt.axis('equal')
    plt.grid()
    plt.title('Map of absolute position errors', y=-0.1)
    plt.xlabel('X [m]')
    plt.ylabel('Y [m]')
    plt.show()

    if return_intermediate_results:
        return true_positions, position_errors


def plot_absolute_position_error_cdf(results, return_intermediate_results=False):
    # FIXME
    position_coordinates = 'position_2d' #['position_x', 'position_y', 'position_z']
    begin_position_coordinates = 'begin_true_position_2d' #['begin_true_position_x', 'begin_true_position_y', 'begin_true_position_z']
    end_position_coordinates = 'end_true_position_2d' #['end_true_position_x', 'end_true_position_y', 'end_true_position_z']

    true_positions = (results[:, begin_position_coordinates] + results[:, end_position_coordinates]) / 2
    position_errors = npla.norm(true_positions - results[:, position_coordinates], axis=1)
    position_errors = np.sort(position_errors)
    n = np.array(range(position_errors.size)) / np.float(position_errors.size)

    plt.plot(position_errors, n)
    plt.grid()
    plt.title('CDF of absolute position error')
    plt.xlabel('Error [m]')
    plt.show()

    if return_intermediate_results:
        return position_errors, n
