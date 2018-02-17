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

import unittest

from smile.helpers import *


class TestHelpers(unittest.TestCase):
    def test_mac_address_to_string(self):
        # Correct case
        self.assertEqual("10-00-00-00-00-02", mac_address_to_string(17592186044418))
        self.assertEqual("9F-FF-E2-32-EA-22", mac_address_to_string(175921360464418))

        # Failures
        with self.assertRaises(ValueError):
            mac_address_to_string("10-00-00-00-00-02")


if __name__ == '__main__':
    unittest.main()
