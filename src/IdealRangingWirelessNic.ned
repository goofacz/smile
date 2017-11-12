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

import inet.linklayer.ideal.IdealWirelessNic;

module IdealRangingWirelessNic like IRangingWirelessNic
{
    parameters:
        @class(smile::IdealRangingWirelessNic);
        string clockModuleRealitivePath = default("^.clock");
        bool enableIdealInterface = default(true);
            	
	gates:
        input radioIn @label(IdealMacFrame);
        input upperLayerIn;
        output upperLayerOut;
        input nicIn @loose;
        output nicOut @loose;
        
    submodules:
    	nic: IdealWirelessNic	{
    	    mac.fullDuplex = false;
    	    mac.bitrate = 10Mbps;
    	    mac.useAck = false;
    	}
    	
	connections allowunconnected:
        radioIn --> nic.radioIn;
        nic.upperLayerOut --> nicOut;
        nicIn --> nic.upperLayerIn;
}
