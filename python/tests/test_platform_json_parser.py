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
import os

from smile.platform.json_parser import *


class TestJsonParser(unittest.TestCase):
    def test_empty_json(self):
        json = JsonParser('smile/python/tests/platform_json_parser/empty.json')
        self.assertIsInstance(json, JsonParser)

    def test_invalid_json(self):
        with self.assertRaises(json.decoder.JSONDecodeError):
            JsonParser('smile/python/tests/platform_json_parser/invalid.json')

    def test_disjoint_jsons(self):
        json = JsonParser('smile/python/tests/platform_json_parser/disjoint_top.json')

        self.assertFalse('import' in json.content)

        self.assertTrue('foo' in json.content)
        self.assertEqual(json.content['foo'], 123)

        self.assertTrue('bar' in json.content)
        self.assertEqual(json.content['bar'], 456)

    def test_cyclical_jsons(self):
        with self.assertRaises(JsonParser.CyclicalImportError):
            JsonParser('smile/python/tests/platform_json_parser/cyclical_import_top.json')

    def test_override_jsons(self):
        json = JsonParser('smile/python/tests/platform_json_parser/override_top.json')

        self.assertFalse('import' in json.content)

        self.assertTrue('bottom_only_number' in json.content)
        self.assertEqual(json.content['bottom_only_number'], 112)

        self.assertTrue('number' in json.content)
        self.assertEqual(json.content['number'], 789)

        self.assertTrue('text' in json.content)
        self.assertEqual(json.content['text'], 'def')

        self.assertTrue('object' in json.content)
        self.assertTrue('collection' in json.content['object'])
        self.assertSequenceEqual(json.content['object']['collection'], [4, 5, 6])

    def test_relative_jsons(self):
        json = JsonParser('smile/python/tests/platform_json_parser/relative_top.json')

        self.assertFalse('import' in json.content)

        self.assertTrue('key' in json.content)
        self.assertEqual(json.content['key'], 'value')
        

if __name__ == '__main__':
    unittest.main()
