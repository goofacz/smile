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
import glob
import os


class Arguments:

    def __init__(self):
        parser = argparse.ArgumentParser(description='Run SMILe simulation')
        parser.add_argument('method_path', type=str, nargs=1, help='path to method')
        parser.add_argument('scenario', type=str, nargs=1, help='scenario name')
        parser.add_argument('config', type=str, nargs=1, help='configuration name')
        parser.add_argument('--ui', type=str, nargs=1, metavar='UI', default='Cmdenv', dest='ui',
                            help='user interface mode (default: Cmdenv)', choices=('Cmdenv', 'Qtenv'),
                            action='store')
        parser.add_argument('--mode', type=str, nargs=1, metavar='mode', default='release', dest='mode',
                            help='executable mode (default: release)', choices=('release', 'debug'),
                            action='store')
        parser.add_argument('--inet', type=str, nargs=1, metavar='path', default='', dest='inet_path',
                            help='path to INET framework', action='store')
        parser.add_argument('--smile', type=str, nargs=1, metavar='path', default='', dest='smile_path',
                            help='path to SMILe simulation framework', action='store')
        parser.add_argument('--exec', type=str, nargs=1, metavar='path', default='', dest='exec_path',
                            help='path to method executable file', action='store')

        self.arguments = parser.parse_args()

        self.method_path = self.__get_method_path()
        self.scenario = self.arguments.scenario[0]
        self.config = self.arguments.config[0]
        self.mode = self.arguments.mode[0]
        self.inet_path = Arguments.__get_framework_path('SMILE_INET_PATH', '--inet', self.arguments.inet_path,
                                                        'INET framework')
        self.smile_path = Arguments.__get_framework_path('SMILE_PATH', '--smile', self.arguments.smile_path,
                                                         'SMILe framework')
        self.ned_paths = Arguments.__get_ned_paths(self.inet_path, 'INET', ['src/inet'])
        self.ned_paths += Arguments.__get_ned_paths(self.smile_path, 'SMILe', ['src', 'simulations'])
        self.ned_paths += Arguments.__get_ned_paths(self.method_path, 'method', ['src', 'simulations'])
        self.ini_path = self.__get_init_path()
        self.executable_path = self.__get_executable_path()

    @staticmethod
    def __get_framework_path(env_name, arg_name, arg_value, location_name):
        path = os.getenv(env_name)

        if arg_value:
            if path:
                print(f'Warning! {env_name} will be overwritten with \'{arg_name}\' argument!')

            path = arg_value[0]

        if not path:
            raise RuntimeError(f'Error! Specify path to {location_name}')

        if not os.path.isdir(path):
            raise RuntimeError(f'\'{path}\' is not valid path {location_name}')

        return path

    @staticmethod
    def __get_ned_paths(root_path, root_name, ned_paths):
        paths = [os.path.join(root_path, ned_path) for ned_path in ned_paths]
        for path in paths:
            package_file_path = os.path.join(path, 'package.ned')
            if (not os.path.isdir(path)) or (not os.path.isfile(package_file_path)):
                raise RuntimeError(f'\'{path}\' is not valid path to NED package in {root_name}')

        return paths

    def __get_method_path(self):
        path = self.arguments.method_path[0]
        if not os.path.isdir(path):
            raise RuntimeError(f'\'{path}\' is not valid path to method')

        return path

    def __get_init_path(self):
        path = os.path.join(self.method_path, 'simulations', self.scenario, 'omnetpp.ini')
        if not os.path.isfile(path):
            raise RuntimeError(f'\'{path}\' is not valid path to simulation INI file')

        return path

    def __get_executable_path(self):
        if self.arguments.exec_path:
            path = self.arguments.exec_path[0]
            if not os.path.isfile(path):
                raise RuntimeError(f'\'{path}\' is not valid executable file')

        if self.mode == 'release':
            executable_name = os.path.basename(self.method_path)
        elif self.mode == 'debug':
            executable_name = os.path.basename(self.method_path) + '_dbg'
        else:
            raise RuntimeError(f'\'{self.mode}\' is unknown mode value')

        search_path_pattern = os.path.join(self.method_path, 'out', '**', executable_name)

        files = glob.glob(search_path_pattern, recursive=True)
        if not files:
            raise RuntimeError(f'Executable file \'{executable_name}\' was not found')
        elif len(files) > 1:
            raise RuntimeError(f'More then one executable files\'{executable_name}\' was not found. '
                               f'Please specify one with \'--exec\' option')

        return files[0]

    def get_opp_runall_args(self):
        cmd_arguments = ['opp_runall', self.executable_path]
        cmd_arguments += ('-f', self.ini_path)
        cmd_arguments += ('-c', self.config)
        cmd_arguments += ('-n', ':'.join(self.ned_paths))
        return cmd_arguments
