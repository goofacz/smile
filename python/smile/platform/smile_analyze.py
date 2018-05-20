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

import argparse
import importlib
import sys

import smile.analysis as sa
import smile.visualization as sv
from smile.platform.json_configuration import *


def __import_method_module(method_module_json):
    module_path = os.path.join(os.environ['SMILE_WORKSPACE_PATH'], method_module_json['path'])
    sys.path.append(module_path)

    return importlib.import_module(method_module_json['name'])


def __parse_cmd_arguments():
    arguments_parser = argparse.ArgumentParser(description='Run SMILe analysis')
    arguments_parser.add_argument('json_file', type=str, nargs=1, help='Path to JSON configuration file')
    arguments_parser.add_argument('results_path', type=str, nargs=1, help='Path to simulation results')
    arguments = arguments_parser.parse_args()
    return arguments.json_file[0], arguments.results_path[0]


if __name__ == '__main__':
    json_file_path, results_path = __parse_cmd_arguments()

    configuration = JsonConfiguration(json_file_path)
    method_module = __import_method_module(configuration['method_module'])

    simulation = method_module.Simulation()
    results, anchors = simulation.run_offline(configuration, results_path)

    unique_results = sa.squeeze_results(results)
    sv.plot_absolute_position_error_cdf(unique_results)
    sv.plot_absolute_position_error_surface(unique_results, anchors)
    sv.plot_absolute_position_error_histogram(unique_results)
