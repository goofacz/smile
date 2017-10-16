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

#include "inet/common/INETDefs.h"

#include "RangingWirelessNic.h"

namespace smile {

Define_Module(RangingWirelessNic);

void RangingWirelessNic::initialize(int stage)
{
  cSimpleModule::initialize(stage);

  if(stage == inet::INITSTAGE_LOCAL)   {
    radio = check_and_cast<inet::physicallayer::IRadio*>(getModuleByPath(".nic.radio"));
    mac = check_and_cast<inet::IdealMac*>(getModuleByPath(".nic.mac"));
  }
  else if(stage == inet::INITSTAGE_ROUTING_PROTOCOLS){
    address.setAddress(mac->par("address").stringValue());
  }
}

} // namespace smile
