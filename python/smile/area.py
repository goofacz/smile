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
import numpy as np
import shapely.geometry as sg


class Area(object):
    class InvalidContentError(ValueError):
        pass

    def __init__(self, file_path):
        file_path = os.path.expanduser(file_path)
        if not os.path.isfile(file_path):
            file_path = os.path.abspath(os.path.join(os.environ['SMILE_WORKSPACE_PATH'], file_path))

        with open(file_path, 'r') as handle:
            content = json.load(handle)

        if 'vertices' not in content:
            raise Area.InvalidContentError('JSON does not contain \'vertices\'')

        self.area = sg.Polygon(content['vertices'])

    def contains(self, point, rtol=1e-5, atol=1e-5):
        if not isinstance(point, sg.Point):
            point = sg.Point(point)

        if self.area.contains(point):
            return True

        distance = self.area.distance(point)
        return np.isclose(distance, 0, rtol=rtol, atol=atol, equal_nan=False)
