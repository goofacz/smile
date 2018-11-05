//
// Copyright (C) 2018 Tomasz Jankowski <t.jankowski AT pwr.edu.pl>
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

#include <inet/linklayer/common/MacAddress.h>
#include <omnetpp.h>

namespace smile {

class IRangingNicDriver
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
  IRangingNicDriver(const IRangingNicDriver& source) = delete;
  IRangingNicDriver(IRangingNicDriver&& source) = delete;
  virtual ~IRangingNicDriver() = default;

  IRangingNicDriver& operator=(const IRangingNicDriver& source) = delete;
  IRangingNicDriver& operator=(IRangingNicDriver&& source) = delete;

  static const omnetpp::simsignal_t txCompletedSignalId;
  static const omnetpp::simsignal_t rxCompletedSignalId;

  virtual inet::MacAddress getMacAddress() const = 0;

 protected:
  IRangingNicDriver() = default;
};

}  // namespace smile
