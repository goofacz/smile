//
// Copyright (C) 2018 Tomasz Jankowski <t.jankowski AT pwr.edu.pl>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package smile;

import inet.applications.contract.IApp;
import inet.linklayer.contract.IWirelessInterface;
import inet.networklayer.common.InterfaceTable;
import inet.node.base.NodeBase;

module WirelessNodeBase extends NodeBase
{
  parameters:
    @networkNode();
    @display("i=device/device");
    string interfaceTableModule = default(absPath(".interfaceTable"));
    string physicalNicType = default("");
    string applicationType = default("");
    string clockType = default("");
    string wirelessInterfaceType = default("");

  gates:
    input radioIn @directIn;

  submodules:
    interfaceTable: InterfaceTable;
    application: <applicationType> like IApp;
    clock: <clockType> like IClock;
    wirelessInterface: <wirelessInterfaceType> like IWirelessInterface;

  connections:
    radioIn --> wirelessInterface.radioIn;
    wirelessInterface.upperLayerIn <-- application.socketOut;
    wirelessInterface.upperLayerOut --> application.socketIn;
}