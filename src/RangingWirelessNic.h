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

class RangingWirelessNic : public omnetpp::cSimpleModule, public omnetpp::cListener
{
 private:
  class FrameHolder
  {
   public:
    using Pair = std::pair<std::unique_ptr<inet::MACFrameBase>, omnetpp::SimTime>;

   public:
    FrameHolder() = default;
    FrameHolder(const FrameHolder& source) = delete;
    FrameHolder(FrameHolder&& source) = delete;
    ~FrameHolder() = default;

    FrameHolder& operator=(const FrameHolder& source) = delete;
    FrameHolder& operator=(FrameHolder&& source) = delete;

    void set(std::unique_ptr<inet::MACFrameBase> newFrame);
    void set(const omnetpp::SimTime& newTimestamp);
    void set(std::unique_ptr<inet::MACFrameBase> newFrame, const omnetpp::SimTime& newTimestamp);

    bool isSet() const;

    Pair release();
    void clear();

   private:
    std::unique_ptr<inet::MACFrameBase> frame;
    omnetpp::SimTime timestamp;
  };

 public:
  RangingWirelessNic() = default;
  RangingWirelessNic(const RangingWirelessNic& source) = delete;
  RangingWirelessNic(RangingWirelessNic&& source) = delete;
  ~RangingWirelessNic() override = default;

  RangingWirelessNic& operator=(const RangingWirelessNic& source) = delete;
  RangingWirelessNic& operator=(RangingWirelessNic&& source) = delete;

  const inet::MACAddress& getMacAddress() const;

  bool transmitFrame(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& clockTimestamp,
                     bool cancelScheduledFrame = false);

  static const omnetpp::simsignal_t transmissionCompletedSignal;
  static const omnetpp::simsignal_t receptionCompletedSignal;

 protected:
  void initialize(int stage) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, omnetpp::cObject* value,
                     omnetpp::cObject* details) override;

 private:
  int numInitStages() const override final;

  void handleTransmissionStateChangedSignal(inet::physicallayer::IRadio::TransmissionState newState);

  void handleReceptionStateChangedSignal(inet::physicallayer::IRadio::ReceptionState newState);

  inet::physicallayer::Radio* radio{nullptr};
  inet::IdealMac* mac{nullptr};
  Clock* clock{nullptr};
  inet::Coord currentPosition;
  inet::MACAddress address;
  FrameHolder scheduledTxFrame;
  FrameHolder lastRxFrame;
  FrameHolder lastTxFrame;
};

}  // namespace smile
