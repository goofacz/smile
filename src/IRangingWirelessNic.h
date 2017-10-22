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

#pragma once

#include <memory>
#include "inet/linklayer/common/MACAddress.h"
#include "omnetpp.h"

namespace smile {

class IRangingWirelessNic
{
 public:
  IRangingWirelessNic(const IRangingWirelessNic& source) = delete;
  IRangingWirelessNic(IRangingWirelessNic&& source) = delete;
  virtual ~IRangingWirelessNic() = default;

  IRangingWirelessNic& operator=(const IRangingWirelessNic& source) = delete;
  IRangingWirelessNic& operator=(IRangingWirelessNic&& source) = delete;

  virtual bool configureDelayedTransmission(const omnetpp::SimTime& delay, bool cancelScheduledOperation) = 0;

  virtual bool configureDelayedReception(const omnetpp::SimTime& delay, bool cancelScheduledOperation) = 0;

  const inet::MACAddress& getMacAddress() const;
  const omnetpp::SimTime& getTransmissionBeginClockTimestamp() const;
  const omnetpp::SimTime& getReceptionBeginClockTimestamp() const;

  static const omnetpp::simsignal_t transmissionCompletedSignal;
  static const omnetpp::simsignal_t receptionCompletedSignal;

 protected:
  IRangingWirelessNic() = default;

  void setAddress(const inet::MACAddress& newAddress);
  void setTransmissionBeginClockTimestamp(const omnetpp::SimTime& newTimestamp);
  void setReceptionBeginClockTimestamp(const omnetpp::SimTime& newTimestamp);

 private:
  inet::MACAddress address;
  omnetpp::SimTime txBeginClockTimestamp;
  omnetpp::SimTime rxBeginClockTimestamp;
};

}  // namespace smile
