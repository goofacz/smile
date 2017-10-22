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
#include "FrameTuple.h"
#include "IRangingWirelessNic.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/linklayer/base/MACFrameBase_m.h"
#include "inet/linklayer/ideal/IdealMac.h"
#include "inet/physicallayer/common/packetlevel/Radio.h"
#include "omnetpp.h"

namespace smile {

class IdealNic : public omnetpp::cSimpleModule, public omnetpp::cListener, public IRangingWirelessNic
{
 private:
  struct Operation
  {
   public:
    Operation() = default;
    Operation(const Operation& source) = delete;
    Operation(Operation&& source) = delete;
    ~Operation() = default;

    Operation& operator=(const Operation& source) = delete;
    Operation& operator=(Operation&& source) = delete;

    const char* getOperationTypeString() const;
    explicit operator bool() const;
    void clear();

    inet::physicallayer::IRadio::RadioMode radioMode;
    std::unique_ptr<inet::MACFrameBase> frame;
    omnetpp::SimTime beginClockTimestamp;
  };

 public:
  IdealNic();
  IdealNic(const IdealNic& source) = delete;
  IdealNic(IdealNic&& source) = delete;
  ~IdealNic();

  IdealNic& operator=(const IdealNic& source) = delete;
  IdealNic& operator=(IdealNic&& source) = delete;

  bool configureDelayedTransmission(const omnetpp::SimTime& delay, bool cancelScheduledOperation) override;

  bool configureDelayedReception(const omnetpp::SimTime& delay, bool cancelScheduledOperation) override;

 private:
  void initialize(int stage) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, const omnetpp::SimTime&,
                     omnetpp::cObject* details) override;

  int numInitStages() const override final;

  void handleMessage(omnetpp::cMessage* message) final;

  void handleLowerLayerInFrame(std::unique_ptr<inet::MACFrameBase> frame);

  void handleUpperLayerInFrame(std::unique_ptr<inet::MACFrameBase> frame);

  void initializeRadio();

  void initializeMac();

  void initializeClock();

  void initializeMobility();

  void handleTransmissionStateChangedSignal(inet::physicallayer::IRadio::TransmissionState newState);

  void handleReceptionStateChangedSignal(inet::physicallayer::IRadio::ReceptionState newState);

  void handleradioModeChangedSignal(inet::physicallayer::IRadio::RadioMode newMode);

  void handleStartScheduleOperationMessage();

  void handleWindowUpdateSignal(const omnetpp::SimTime& windowEndClockTimestamp);

  bool scheduleOperation(inet::physicallayer::IRadio::RadioMode mode, const omnetpp::SimTime& delay,
                         bool cancelScheduledOperation);

  void sendStartScheduleOperationMessage();

  inet::physicallayer::Radio* radio{nullptr};
  inet::IdealMac* mac{nullptr};
  Clock* clock{nullptr};
  inet::Coord currentPosition;

  Operation scheduledOperation;

  std::unique_ptr<omnetpp::cMessage> startScheduleOperationMessage;
};

}  // namespace smile
