//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package smile;

//
// Default measurements logger that gathers logs from all radio nodes in simulations. It supports
// logging to file in CSV format.
//
simple MeasurementsLogger
{
    parameters:
        @class(smile::MeasurementsLogger);
        @display("i=block/buffer");
        string directoryPath = default(""); // Directory when output file should be stored
        string fileName = default(""); // Output file name
        string format = default("CSV"); // Output format, at the moment only CSV is supported
        bool overwrite = default(false); // Whenever file should be overwritten or not
}