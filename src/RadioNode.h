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
#include "Clock.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/linklayer/base/MACFrameBase_m.h"
#include "inet/linklayer/ideal/IdealMac.h"
#include "inet/physicallayer/common/packetlevel/Radio.h"
#include "omnetpp.h"

namespace smile {

class RadioNode : public omnetpp::cSimpleModule, public omnetpp::cListener
{
 private:
  using ScheduledTxFrame = std::pair<std::unique_ptr<inet::MACFrameBase>, omnetpp::SimTime>;

 public:
  RadioNode() = default;
  RadioNode(const RadioNode& source) = delete;
  RadioNode(RadioNode&& source) = delete;
  ~RadioNode() override = default;

  RadioNode& operator=(const RadioNode& source) = delete;
  RadioNode& operator=(RadioNode&& source) = delete;

  const inet::MACAddress& getMacAddress() const;
  const inet::Coord& getCurrentPosition() const;

  bool transmitFrame(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& clockTimestamp,
                     bool cancelScheduledFrame = false);

 protected:
  void initialize(int stage) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, omnetpp::cObject* value,
                     omnetpp::cObject* details) override;

 private:
  int numInitStages() const override final;

  void handleMobilityStateChanged(omnetpp::cObject* value);

  void setScheduledTxFrame(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& clockTimestamp);

  bool hasScheduledTxFrame() const;

  ScheduledTxFrame takeScheduledTxFrame();

  inet::physicallayer::Radio* radio{nullptr};
  inet::IdealMac* mac{nullptr};
  Clock* clock{nullptr};
  inet::Coord currentPosition;
  inet::MACAddress address;

  ScheduledTxFrame scheduledTxFrame;
};

}  // namespace smile
