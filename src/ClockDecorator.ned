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
// This module interface should be implemented by all modules that use clock modules. C++ class
// corresponding to this interface extends OMNET++ API and makes sure that all time-related
// functions like scheduleAt() or sendDelayed() will measure time according clock module instead
// of oglobal simulation clock.
//
moduleinterface ClockDecorator
{
	parameters:
	    string clockModule; // Path to clock relative to this module
}
