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

#include "ClockDecorator.h"
#include "IRangingWirelessNic.h"
#include "IdealRangingRadioFrame.h"
#include "omnetpp.h"

namespace smile {

class IdealRangingWirelessNic : public ClockDecorator<omnetpp::cSimpleModule>, public IRangingWirelessNic
{
 public:
  IdealRangingWirelessNic() = default;
  IdealRangingWirelessNic(const IdealRangingWirelessNic& source) = delete;
  IdealRangingWirelessNic(IdealRangingWirelessNic&& source) = delete;
  ~IdealRangingWirelessNic() = default;

  IdealRangingWirelessNic& operator=(const IdealRangingWirelessNic& source) = delete;
  IdealRangingWirelessNic& operator=(IdealRangingWirelessNic&& source) = delete;

 private:
  void handleIncommingMessage(omnetpp::cMessage* newMessage) override final;

  void handleUpperFrame(std::unique_ptr<IdealRangingRadioFrame> frame);

  void handleLowerFrame(std::unique_ptr<IdealRangingRadioFrame> frame);
};

}  // namespace smile
