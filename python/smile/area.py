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

import numpy as np
import shapely.geometry as sg


class Area(object):
    def __init__(self, area):
        self.area = area

    @staticmethod
    def load_json(file):
        if isinstance(file, str):
            with open(file, 'r') as handle:
                content = json.load(handle)
        else:
            content = json.load(file)

        if 'vertices' not in content:
            raise ValueError('JSON does not contain \'vertices\'')

        area = sg.Polygon(content['vertices'])

        return Area(area)

    def contains(self, point, rtol=1e-5, atol=1e-5):
        if self.area.contains(point):
            return True

        distance = self.area.distance(point)
        return np.isclose(distance, 0, rtol=rtol, atol=atol, equal_nan=False)
