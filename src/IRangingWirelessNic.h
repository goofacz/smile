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

#include <omnetpp.h>
#include <inet/linklayer/base/MACFrameBase_m.h>

namespace smile {

class IRangingWirelessNic
{
 public:
  enum class IdealTxCompletionStatus
  {
    SUCCESS
  };

  enum class IdealRxCompletionStatus
  {
    SUCCESS
  };

 public:
  IRangingWirelessNic(const IRangingWirelessNic& source) = delete;
  IRangingWirelessNic(IRangingWirelessNic&& source) = delete;
  virtual ~IRangingWirelessNic() = default;

  IRangingWirelessNic& operator=(const IRangingWirelessNic& source) = delete;
  IRangingWirelessNic& operator=(IRangingWirelessNic&& source) = delete;

  static const omnetpp::simsignal_t txCompletedSignalId;
  static const omnetpp::simsignal_t rxCompletedSignalId;

  virtual inet::MACAddress getMacAddress() const = 0;

 protected:
  IRangingWirelessNic() = default;
};

}  // namespace smile
