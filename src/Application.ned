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
// Base module for applications running on radio nodes intended for inheritance. Application should
// be used to process and generate MAC frames on each radio node.
//
simple Application like IApplication
{
    parameters:
        @class(smile::Application);
        @display("i=block/app");
        string name = default(""); // Name of this application used in GUI (optional)
        string nicDriverModule = default("^.nicDriver"); // Path to nicDriver relative to this module
        string measurementsLoggerModule = default("measurementsLogger"); // Path to measurementsLogger relative to this module
        string clockModule = default("^.clock"); // Path to clock relative to this module
        string mobilityModule = default("^.mobility"); // Path to mobility relative to this module

	gates:
	    // Used only by IdealApplication
	    input in @loose; // Input for MAC frames, used by @see IdealApplication
	    output out @loose; // Output for MAC frames, used by @see IdealApplication
}
