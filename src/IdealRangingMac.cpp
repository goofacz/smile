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

#include "IdealRangingMac.h"

namespace smile {

Define_Module(IdealRangingMac);

void IdealRangingMac::initializeMACAddress()
{
  const auto address = par("address").stdstringValue();
  const auto isInteger = [&address] { return address.find_first_not_of("0123456789") == std::string::npos; };

  if (isInteger()) {
    const uint64_t numericAddress = 0xCCAA00000000 + std::stol(address);
    const auto macAddress = inet::MACAddress{numericAddress};
    par("address").setStringValue(macAddress.str());
  }

  IdealMac::initializeMACAddress();
}

}  // namespace smile
