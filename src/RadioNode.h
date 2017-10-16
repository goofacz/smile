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

#include <functional>
#include "IApplication.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/linklayer/common/MACAddress.h"
#include "omnetpp.h"

namespace smile {

class RadioNode : public omnetpp::cSimpleModule, public omnetpp::cListener
{
 public:
  RadioNode() = default;
  RadioNode(const RadioNode& source) = delete;
  RadioNode(RadioNode&& source) = delete;
  ~RadioNode() override = default;

  RadioNode& operator=(const RadioNode& source) = delete;
  RadioNode& operator=(RadioNode&& source) = delete;

  inet::MACAddress getMACAddress() const;
  const inet::Coord& getCurrentPosition() const;

 protected:
  void initialize(int stage) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, omnetpp::cObject* value,
                     omnetpp::cObject* details) override;

 private:
  int numInitStages() const override;

  void handleMobilityStateChanged(omnetpp::cObject* value);

  IApplication* iApplication{nullptr};
  inet::Coord currentPosition;
};

}  // namespace smile
