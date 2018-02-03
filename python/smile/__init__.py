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

import sys

if 'unittests' not in sys.modules:
    # There's no need to display interactive plots
    import matplotlib
    matplotlib.use('AGG')

import smile.analysis as analysis
import smile.frames as frames
import smile.helpers as helpers
import smile.nodes as nodes
import smile.results as results
