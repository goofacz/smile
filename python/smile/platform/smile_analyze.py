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
import os
import importlib.util


def __import_method_module(method_path, module_name):
    module_path = os.path.join(method_path, 'python', module_name, 'algorithm.py')
    method_module_spec = importlib.util.spec_from_file_location(module_name, module_path)
    method_module = importlib.util.module_from_spec(method_module_spec)
    method_module_spec.loader.exec_module(method_module)
    pass


def __parse_cmd_arguments():
    arguments_parser = argparse.ArgumentParser(description='Run SMILe analysis')
    arguments_parser.add_argument('method_path', type=str, nargs=1,
                                  help='path to method')
    arguments_parser.add_argument('module_name', type=str, nargs=1,
                                  help='module name')
    arguments_parser.add_argument('results_path', type=str, nargs=1,
                                  help='path to simulation results')
    arguments = arguments_parser.parse_args()

    method_path = arguments.method_path[0]
    if not os.path.isdir(method_path):
        raise RuntimeError(f'{method_path} is not valid path to directory')

    return method_path, arguments.module_name[0]


if __name__ == '__main__':
    method_path, module_name = __parse_cmd_arguments()
    __import_method_module(method_path, module_name)