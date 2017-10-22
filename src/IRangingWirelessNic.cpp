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

#include "IRangingWirelessNic.h"
#include <memory>

namespace smile {

const omnetpp::simsignal_t IRangingWirelessNic::transmissionCompletedSignal{
    omnetpp::cComponent::registerSignal("transmissionCompleted")};

const inet::MACAddress& IRangingWirelessNic::getMacAddress() const
{
  return address;
}

const omnetpp::SimTime& IRangingWirelessNic::getTransmissionBeginClockTimestamp() const
{
  return txBeginClockTimestamp;
}

const omnetpp::SimTime& IRangingWirelessNic::getReceptionBeginClockTimestamp() const
{
  return rxBeginClockTimestamp;
}

void IRangingWirelessNic::setAddress(const inet::MACAddress& newAddress)
{
  address = newAddress;
}

void IRangingWirelessNic::setTransmissionBeginClockTimestamp(const omnetpp::SimTime& newTimestamp)
{
  txBeginClockTimestamp = newTimestamp;
}

void IRangingWirelessNic::setReceptionBeginClockTimestamp(const omnetpp::SimTime& newTimestamp)
{
  rxBeginClockTimestamp = newTimestamp;
}

}  // namespace smile
