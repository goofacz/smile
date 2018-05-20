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
import subprocess


def __parse_cmd_arguments():
    arguments_parser = argparse.ArgumentParser(description='Run SMILe simulation')
    arguments_parser.add_argument('method_path', type=str, nargs=1,
                                  help='path to method')
    arguments_parser.add_argument('ini_path', type=str, nargs=1,
                                  help='path to simulation INI file')
    arguments_parser.add_argument('config', type=str, nargs=1,
                                  help='configuration name (specified in INI file)')
    arguments_parser.add_argument('-u', type=str, nargs=1, metavar='UI', default='Cmdenv', dest='ui',
                                  help='user interface mode (default: Cmdenv)', choices=('Cmdenv', 'Qtenv'))

    arguments = arguments_parser.parse_args()

    method_path = arguments.method_path[0]
    if not os.path.isdir(method_path):
        raise RuntimeError(f'{method_path} is not valid path to directory')

    ini_path = arguments.ini_path[0]
    if not os.path.isfile(ini_path):
        raise RuntimeError(f'{ini_path} does not exist or is not a regular file')

    method_path = os.path.normpath(os.path.expanduser(method_path))
    ini_path = os.path.normpath(os.path.expanduser(ini_path))

    return method_path, ini_path, arguments.config[0], arguments.ui


def __get_smile_workspace_path():
    try:
        return os.environ['SMILE_WORKSPACE_PATH']
    except KeyError:
        raise EnvironmentError('Environment variable SMILE_WORKSPACE_PATH is not available, please set it first')


def __prepare_image_path():
    return os.path.join(__get_smile_workspace_path(), 'inet/images')


def __prepare_ned_paths(method_path):
    workspace_path = __get_smile_workspace_path()
    ned_paths = (
        'smile/src',
        'smile/simulations',
        'inet/src',
        'inet/examples',
        'inet/tutorials',
        'inet/showcases'
    )

    ned_paths = [os.path.join(workspace_path, ned_path) for ned_path in ned_paths]
    ned_paths.append(os.path.join(method_path, 'src'))
    ned_paths.append(os.path.join(method_path, 'simulations'))
    return ned_paths


def __prepare_library_paths(method_path):
    workspace_path = __get_smile_workspace_path()
    library_paths = (
        'inet/src/INET',
        'smile/src/smile'
    )

    library_paths = [os.path.join(workspace_path, library_path) for library_path in library_paths]

    method_name = os.path.split(method_path)[-1]
    library_paths.append(os.path.join(method_path, 'src', method_name))

    return library_paths


def __run_simulation(ini_path, config_name, ui, ned_paths, library_paths, image_path):
    arguments = ['opp_run', '-m', ini_path]

    arguments += ('-c', config_name)
    arguments += ('-u', ui)
    arguments += ('--image-path', image_path)
    arguments += ('-n', ':'.join(ned_paths))
    for library_path in library_paths:
        arguments += ('-l', library_path)

    return subprocess.call(arguments)


if __name__ == '__main__':
    method_path, ini_path, config_name, ui = __parse_cmd_arguments()

    ned_paths = __prepare_ned_paths(method_path)
    library_paths = __prepare_library_paths(method_path)
    image_path = __prepare_image_path()

    return_code = __run_simulation(ini_path, config_name, ui, ned_paths, library_paths, image_path)
    exit(return_code)
