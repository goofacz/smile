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

#include "IdealRangingWirelessNic.h"
#include "utilities.h"

namespace smile {

Define_Module(IdealRangingWirelessNic);

void IdealRangingWirelessNic::handleIncommingMessage(omnetpp::cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};

  const auto arrivalGate = message->getArrivalGate();
  if (arrivalGate == gate("upperLayerIn")) {
    handleUpperFrame(dynamic_unique_ptr_cast<IdealRangingRadioFrame>(std::move(message)));
  }
  else if (arrivalGate == gate("lowerLayerIn")) {
    handleLowerFrame(dynamic_unique_ptr_cast<IdealRangingRadioFrame>(std::move(message)));
  }
  else {
    throw cRuntimeError{"Message \"%s\" arrived at unexpected gate: \"%s\"", message->getName(),
                        arrivalGate->getName()};
  }
}

void IdealRangingWirelessNic::handleUpperFrame(std::unique_ptr<IdealRangingRadioFrame> frame)
{
  // TODO
}

void IdealRangingWirelessNic::handleLowerFrame(std::unique_ptr<IdealRangingRadioFrame> frame)
{
  // TODO
}

}  // namespace smile
