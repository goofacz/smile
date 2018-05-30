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

from smile.results import Results, Result


def _squeeze_results_by_mac_address(results, averaging_method):
    # FIXME
    # position_coordinates, begin_position_coordinates, end_position_coordinates = results.determine_dimensions()

    # We want to have single result per mobile node
    unique_mac_addresses = np.unique(results[:, "mac_address"])
    unique_results = [] # Results.create_array(len(unique_mac_addresses))

    for i in range(0, len(unique_mac_addresses)):
        mac_address = unique_mac_addresses[i]
        tmp_results = results[results[:, "mac_address"] == mac_address]

        unique_result = Result()
        if averaging_method == "mean":
            unique_result.position_x = np.mean(tmp_results[:, 'position_x'], axis=0)
            unique_result.position_y = np.mean(tmp_results[:, 'position_y'], axis=0)
            unique_result.position_z = np.mean(tmp_results[:, 'position_z'], axis=0)
            unique_result.begin_true_position_x = np.mean(tmp_results[:, 'begin_true_position_x'], axis=0)
            unique_result.begin_true_position_y = np.mean(tmp_results[:, 'begin_true_position_y'], axis=0)
            unique_result.begin_true_position_z = np.mean(tmp_results[:, 'begin_true_position_z'], axis=0)
            unique_result.end_true_position_x = np.mean(tmp_results[:, 'end_true_position_x'], axis=0)
            unique_result.end_true_position_y = np.mean(tmp_results[:, 'end_true_position_y'], axis=0)
            unique_result.end_true_position_z = np.mean(tmp_results[:, 'end_true_position_z'], axis=0)
        elif averaging_method == "max":
            unique_result.position_x = np.max(tmp_results[:, 'position_x'], axis=0)
            unique_result.position_y = np.max(tmp_results[:, 'position_y'], axis=0)
            unique_result.position_z = np.max(tmp_results[:, 'position_z'], axis=0)
            unique_result.begin_true_position_x = np.max(tmp_results[:, 'begin_true_position_x'], axis=0)
            unique_result.begin_true_position_y = np.max(tmp_results[:, 'begin_true_position_y'], axis=0)
            unique_result.begin_true_position_z = np.max(tmp_results[:, 'begin_true_position_z'], axis=0)
            unique_result.end_true_position_x = np.max(tmp_results[:, 'end_true_position_x'], axis=0)
            unique_result.end_true_position_y = np.max(tmp_results[:, 'end_true_position_y'], axis=0)
            unique_result.end_true_position_z = np.max(tmp_results[:, 'end_true_position_z'], axis=0)
        elif averaging_method == "min":
            unique_result.position_x = np.min(tmp_results[:, 'position_x'], axis=0)
            unique_result.position_y = np.min(tmp_results[:, 'position_y'], axis=0)
            unique_result.position_z = np.min(tmp_results[:, 'position_z'], axis=0)
            unique_result.begin_true_position_x = np.min(tmp_results[:, 'begin_true_position_x'], axis=0)
            unique_result.begin_true_position_y = np.min(tmp_results[:, 'begin_true_position_y'], axis=0)
            unique_result.begin_true_position_z = np.min(tmp_results[:, 'begin_true_position_z'], axis=0)
            unique_result.end_true_position_x = np.min(tmp_results[:, 'end_true_position_x'], axis=0)
            unique_result.end_true_position_y = np.min(tmp_results[:, 'end_true_position_y'], axis=0)
            unique_result.end_true_position_z = np.min(tmp_results[:, 'end_true_position_z'], axis=0)
        else:
            raise ValueError(
                "Unknown \"averaging_method\" value passed to squeeze_results(): {0}".format(averaging_method))

        unique_result.mac_address = mac_address
        unique_result.position_dimensions = tmp_results[0, 'position_dimensions'] # FIXME
        unique_results.append(unique_result)

    unique_results = Results.create_array(unique_results)
    return unique_results


def squeeze_results(results, grouping_method="mac_address", averaging_method="mean"):
    if grouping_method == "mac_address":
        return _squeeze_results_by_mac_address(results, averaging_method)
    else:
        raise ValueError(
            "Unknown \"grouping_method\" value passed to squeeze_results(): {0}".format(grouping_method))
