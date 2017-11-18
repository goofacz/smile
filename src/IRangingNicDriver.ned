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
// Wireless NIC driver provides an unified "Ideal" interface for all kinds of
// wireless NICs. The goal is to hide all hardware detailes (setting up NIC,
// preparing and handling TX/RX operations etc.). Drivers are used together with
// "Ideal" applications @see IdealApplication.
//
moduleinterface IRangingNicDriver
{
    parameters:
        @display("i=block/layer");
        @signal[txCompleted](type=smile::IdealTxCompletion); // Emits whenever transmission is completed
        @signal[rxCompleted](type=smile::IdealRxCompletion); // Emits whenever reception is completed

	gates:
        input applicationIn; // Input for MAC frames from applications based on @see IdealApplication
        output applicationOut; // Output for MAC frames to applications based on @see IdealApplication
        input nicIn; // Input for IdealWirelessNic
        output nicOut; // Output to IdealWirelessNic
}