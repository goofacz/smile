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

from smile.results import Results


def obtain_unique_results(results):
    position_coordinates, begin_position_coordinates, end_position_coordinates = results.determine_dimensions()

    # We want to have single result per mobile node
    unique_mac_addresses = np.unique(results[:, "mac_address"])
    unique_results = Results.create_array(len(unique_mac_addresses))

    for i in range(0, len(unique_mac_addresses)):
        mac_address = unique_mac_addresses[i]
        tmp_results = results[results[:, "mac_address"] == mac_address]

        unique_results[i, :] = tmp_results[0, :]
        # TODO Implement more ways of "squeezing" multiple results into one
        unique_results[i, begin_position_coordinates] = np.mean(tmp_results[:, begin_position_coordinates], axis=0)
        unique_results[i, end_position_coordinates] = np.mean(tmp_results[:, end_position_coordinates], axis=0)
        unique_results[i, position_coordinates] = np.mean(tmp_results[:, position_coordinates], axis=0)

    return unique_results
