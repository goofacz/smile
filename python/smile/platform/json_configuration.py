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
import os

import jsonmerge


class JsonConfiguration(object):
    class CyclicalImportError(ValueError):
        pass

    def __init__(self, file_path):
        self._smile_workspace_path = os.environ['SMILE_WORKSPACE_PATH']

        file_path = os.path.expanduser(file_path)
        if not os.path.isfile(file_path):
            file_path = self._compose_absolute_file_path(self._smile_workspace_path, file_path)

        self._content = JsonConfiguration._load_file(file_path)
        self._imported_files = [file_path]

        self._import_files(file_path, self._content)

    def __getitem__(self, key):
        return self._content[self.__keytransform__(key)]

    def __setitem__(self, key, value):
        self._content[self.__keytransform__(key)] = value

    def __delitem__(self, key):
        del self._content[self.__keytransform__(key)]

    def __iter__(self):
        return iter(self._content)

    def __len__(self):
        return len(self._content)

    def __keytransform__(self, key):
        return key

    def _import_files(self, absolute_file_path, content):
        if 'import' not in content:
            return

        # Get directory where analyzed JSON file is located
        base_directory_path = os.path.dirname(absolute_file_path)

        # "import" shouldn't be visible to end user
        imported_files = content['import']
        del content['import']

        # JSONs merging order is from bottom to top
        merged_imported_content = {}
        for imported_file in imported_files:
            absolute_imported_file_path = self._compose_absolute_file_path(base_directory_path, imported_file)
            if absolute_imported_file_path in self._imported_files:
                raise JsonConfiguration.CyclicalImportError(
                    f'Cyclical import occurred, file {imported_file} was already imported')

            imported_content = JsonConfiguration._load_file(absolute_imported_file_path)
            self._imported_files.append(absolute_imported_file_path)
            self._import_files(absolute_imported_file_path, imported_content)

            merged_imported_content = jsonmerge.merge(merged_imported_content, imported_content)

        self._content = jsonmerge.merge(merged_imported_content, self._content)

    def as_dict(self):
        return self._content

    @staticmethod
    def _load_file( file_path):
        with open(file_path, 'r') as handle:
            return json.load(handle)

    def _compose_absolute_file_path(self, base_directory_path, file_path):
        absolute_file_path = os.path.abspath(os.path.join(base_directory_path, file_path))
        if not os.path.isfile(absolute_file_path):
            absolute_file_path = os.path.abspath(os.path.join(self._smile_workspace_path, file_path))

        return absolute_file_path
