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
#include "inet/common/geometry/common/Coord.h"
#include "inet/linklayer/base/MACFrameBase_m.h"
#include "inet/linklayer/ideal/IdealMac.h"
#include "inet/physicallayer/common/packetlevel/Radio.h"
#include "omnetpp.h"

namespace smile {

class RangingWirelessNic : public omnetpp::cSimpleModule, public omnetpp::cListener
{
 private:
  class Operation
  {
   public:
    enum class Type
    {
      TX,
      RX
    };

   public:
    Operation() = default;
    Operation(const Operation& source) = delete;
    Operation(Operation&& source) = delete;
    ~Operation() = default;

    Operation& operator=(const Operation& source) = delete;
    Operation& operator=(Operation&& source) = default;
    explicit operator bool() const;

    void set(std::unique_ptr<inet::MACFrameBase> newFrame);
    void set(const omnetpp::SimTime& newTimestamp);
    void set(Type newType, std::unique_ptr<inet::MACFrameBase> newFrame, const omnetpp::SimTime& newTimestamp);

    Type getType() const;
    const char* getTypeAsString() const;
    inet::physicallayer::Radio::RadioMode getRadioMode() const;
    const std::unique_ptr<inet::MACFrameBase>& getFrame() const;
    const omnetpp::SimTime& getTimestamp() const;

    std::unique_ptr<FrameTuple> release();
    void clear();

   private:
    Type type{Type::TX};
    std::unique_ptr<inet::MACFrameBase> frame;
    omnetpp::SimTime timestamp;
  };

 public:
  RangingWirelessNic();
  RangingWirelessNic(const RangingWirelessNic& source) = delete;
  RangingWirelessNic(RangingWirelessNic&& source) = delete;
  ~RangingWirelessNic();

  RangingWirelessNic& operator=(const RangingWirelessNic& source) = delete;
  RangingWirelessNic& operator=(RangingWirelessNic&& source) = delete;

  const inet::MACAddress& getMacAddress() const;

  bool scheduleTransmission(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& delay,
                            bool cancelScheduledOperation = false);

  bool scheduleReception(const omnetpp::SimTime& delay, bool cancelScheduledOperation = false);

  static const omnetpp::simsignal_t transmissionCompletedSignal;
  static const omnetpp::simsignal_t receptionCompletedSignal;

 private:
  void initialize(int stage) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, const omnetpp::SimTime&,
                     omnetpp::cObject* details) override;

  int numInitStages() const override final;

  void handleMessage(omnetpp::cMessage* message) final;

  void handleReceivedFrame(std::unique_ptr<inet::MACFrameBase> frame);

  void initializeRadio();

  void initializeMac();

  void initializeClock();

  void initializeMobility();

  void handleTransmissionStateChangedSignal(inet::physicallayer::IRadio::TransmissionState newState);

  void handleReceptionStateChangedSignal(inet::physicallayer::IRadio::ReceptionState newState);

  void handleradioModeChangedSignal(inet::physicallayer::IRadio::RadioMode newMode);

  bool scheduleOperation(Operation::Type type, std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& delay,
                         bool cancelScheduledOperation);

  void handleTransmisionCompletion();

  void handleReceptionCompletion();

  void handleStartScheduleOperationMessage();

  void handleWindowUpdateSignal(const omnetpp::SimTime& windowEndClockTimestamp);

  inet::physicallayer::Radio* radio{nullptr};
  inet::IdealMac* mac{nullptr};
  Clock* clock{nullptr};
  inet::Coord currentPosition;
  inet::MACAddress address;

  Operation scheduledOperation;
  Operation lastRxOperation;
  Operation lastTxOperation;

  std::unique_ptr<omnetpp::cMessage> startScheduleOperationMessage;
};

}  // namespace smile
