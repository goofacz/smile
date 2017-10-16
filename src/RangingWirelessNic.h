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

#include "omnetpp.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/linklayer/ideal/IdealMac.h"

namespace smile {

class RangingWirelessNic : public omnetpp::cSimpleModule
{
 public:
  RangingWirelessNic() = default;
  RangingWirelessNic(const RangingWirelessNic& source) = delete;
  RangingWirelessNic(RangingWirelessNic&& source) = delete;
  ~RangingWirelessNic() override = default;

  RangingWirelessNic& operator=(const RangingWirelessNic& source) = delete;
  RangingWirelessNic& operator=(RangingWirelessNic&& source) = delete;

 protected:
  void initialize(int stage) override;

 private:
  inet::physicallayer::IRadio* radio{nullptr};
  inet::IdealMac* mac{nullptr};
  inet::MACAddress address;
};

}  // namespace smile
