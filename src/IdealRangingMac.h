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

#include <inet/linklayer/ideal/IdealMac.h>
#include <omnetpp.h>

namespace smile {

class IdealRangingMac : public inet::IdealMac
{
 public:
  IdealRangingMac() = default;
  IdealRangingMac(const IdealRangingMac& source) = delete;
  IdealRangingMac(IdealRangingMac&& source) = delete;
  ~IdealRangingMac() override = default;

  IdealRangingMac& operator=(const IdealRangingMac& source) = delete;
  IdealRangingMac& operator=(IdealRangingMac&& source) = delete;

 private:
  void initializeMACAddress() override;
};

}  // namespace smile
