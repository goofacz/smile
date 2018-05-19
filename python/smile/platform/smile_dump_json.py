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

from smile.platform.json_configuration import *


def __parse_cmd_arguments():
    arguments_parser = argparse.ArgumentParser(description='Imports all JSON configuration files and prints them as \
                                                           single JSON file')
    arguments_parser.add_argument('json_file', type=str, nargs=1, help='Path to main JSON configuration file')
    arguments = arguments_parser.parse_args()
    return arguments.json_file[0]


if __name__ == '__main__':
    json_file_path = __parse_cmd_arguments()
    configuration = JsonConfiguration(json_file_path)
    json_text = json.dumps(configuration, indent=2)
    print(json_text)
