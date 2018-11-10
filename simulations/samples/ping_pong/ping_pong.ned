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

package smile.simulations.samples.ping_pong;

import smile.wireless_nodes.AckingWirelessNode;
import inet.mobility.static.StationaryMobility;
import inet.physicallayer.unitdisk.UnitDiskRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network ping_pong
{
    parameters:
        @display("bgb=75,75");
        int mobilesNumber = default(0);
        int anchorsNumber = default(0);

    submodules:
        radioMedium: UnitDiskRadioMedium {
            @display("p=181,168");
        }

        visualizer: IntegratedCanvasVisualizer {
            @display("p=258,166");
        }

        ping_node: AckingWirelessNode;
        
        pong_node: AckingWirelessNode;
}