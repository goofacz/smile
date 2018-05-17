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

import json
import jsonmerge
import os


class JsonParser(object):
    def __init__(self, file_path):
        self._base_directory = os.environ['SMILE_WORKSPACE_PATH']
        self._imported_files = [self._compose_absolute_file_path(file_path)]

        self.content = self._load_file(file_path)

        self._import_files(self.content)

    def _import_files(self, content):
        if 'import' not in content:
            return

        imported_files = content['import']
        del content['import']

        merged_imported_content = {}
        for imported_file in imported_files:
            absolute_imported_file_path = self._compose_absolute_file_path(imported_file)
            if absolute_imported_file_path in self._imported_files:
                raise ValueError(f'Cyclical import occurred, file {imported_file} was already imported')
                pass

            imported_content = self._load_file(imported_file)
            self._imported_files.append(absolute_imported_file_path)
            self._import_files(imported_content)

            merged_imported_content = jsonmerge.merge(merged_imported_content, imported_content)

        self.content = jsonmerge.merge(merged_imported_content, self.content)

    def _load_file(self, file_path):
        absolute_file_path = self._compose_absolute_file_path(file_path)
        with open(absolute_file_path, 'r') as handle:
            return json.load(handle)

    def _compose_absolute_file_path(self, file_path):
        return os.path.abspath(os.path.join(self._base_directory, file_path))
