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

#include <cassert>

#include "inet/common/INETDefs.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"

#include "RadioNode.h"

namespace smile {

Define_Module(RadioNode);

void RadioNode::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_PHYSICAL_ENVIRONMENT_2) {
    setupMobilityListeners();
  }
}

int RadioNode::numInitStages() const
{
  return inet::INITSTAGE_LINK_LAYER_2 + 1;
}

inet::MACAddress RadioNode::getMACAddress() const
{
  const auto mac = getModuleByPath(".nic.mac");
  assert(mac);
  const auto& address = mac->par("address");
  assert(address.getType() == cPar::STRING);
  return inet::MACAddress(address.stringValue());
}

const inet::Coord& RadioNode::getCurrentPosition() const
{
  return currentPosition;
}

void RadioNode::mobilityStateChangedCallback(omnetpp::cComponent* source,
                                             simsignal_t signalID,
                                             omnetpp::cObject* value,
                                             omnetpp::cObject* details)
{
  auto mobility = check_and_cast<inet::IMobility*>(value);
  assert(mobility);
  currentPosition = mobility->getCurrentPosition();
  EV_DETAIL << "Current position: " << currentPosition << endl;
}

void RadioNode::setupMobilityListeners()
{
  auto handler = [this](auto source, auto signalID, auto value, auto details) {
    this->mobilityStateChangedCallback(source, signalID, value, details);
  };
  mobilityStateChangedListener = handler;

  auto mobility = getModuleByPath(".mobility");
  assert(mobility);
  mobility->subscribe("mobilityStateChanged", &mobilityStateChangedListener);

  auto iMobility = check_and_cast<inet::IMobility*>(mobility);
  assert(mobility);
  currentPosition = iMobility->getCurrentPosition();
  EV_DETAIL << "Current position: " << currentPosition << endl;
}

}  // namespace smile
