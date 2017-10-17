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
#include "inet/linklayer/base/MACProtocolBase.h"
#include "inet/physicallayer/common/packetlevel/Radio.h"

#include "RangingWirelessNic.h"

namespace smile {

Define_Module(RangingWirelessNic);

const inet::MACAddress& RangingWirelessNic::getMacAddress() const
{
  return address;
}

bool RangingWirelessNic::transmitFrame(std::unique_ptr<inet::MACFrameBase> frame,
                                       const omnetpp::SimTime& clockTimestamp, bool cancelScheduledFrame)
{
  // TODO

  return true;
}

void RangingWirelessNic::initialize(int stage)
{
  cSimpleModule::initialize(stage);

  if (stage == inet::INITSTAGE_PHYSICAL_ENVIRONMENT_2) {
    radio = check_and_cast<inet::physicallayer::Radio*>(getModuleByPath(".nic.radio"));
    radio->subscribe(inet::physicallayer::Radio::transmissionStateChangedSignal, this);
    radio->subscribe(inet::physicallayer::Radio::receptionStateChangedSignal, this);

    mac = check_and_cast<inet::IdealMac*>(getModuleByPath(".nic.mac"));
    address.setAddress(mac->par("address").stringValue());
    EV_DETAIL << "Radio node MAC address: " << address << endl;

    clock = check_and_cast<Clock*>(getModuleByPath(".clock"));

    auto mobility = check_and_cast<omnetpp::cComponent*>(getModuleByPath(".mobility"));
    mobility->subscribe(inet::IMobility::mobilityStateChangedSignal, this);

    auto iMobility = check_and_cast<inet::IMobility*>(mobility);
    currentPosition = iMobility->getCurrentPosition();
    EV_DETAIL << "Current position: " << currentPosition << endl;
  }
}

void RangingWirelessNic::receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t signalID, long value,
                                       omnetpp::cObject*)
{
  // TODO
}

int RangingWirelessNic::numInitStages() const
{
  return inet::INITSTAGE_LINK_LAYER_2 + 1;
}

void RangingWirelessNic::FrameHolder::set(std::unique_ptr<inet::MACFrameBase> newFrame)
{
  frame = std::move(newFrame);
}

void RangingWirelessNic::FrameHolder::set(const omnetpp::SimTime& newTimestamp)
{
  timestamp = newTimestamp;
}

void RangingWirelessNic::FrameHolder::set(std::unique_ptr<inet::MACFrameBase> newFrame,
                                          const omnetpp::SimTime& newTimestamp)
{
  set(std::move(newFrame));
  set(newTimestamp);
}

bool RangingWirelessNic::FrameHolder::isSet() const
{
  return frame && timestamp > 0;
}

RangingWirelessNic::FrameHolder::Pair RangingWirelessNic::FrameHolder::release()
{
  auto result = std::make_pair(std::move(frame), timestamp);
  clear();
  return result;
}

void RangingWirelessNic::FrameHolder::clear()
{
  set(nullptr, 0);
}

}  // namespace smile
