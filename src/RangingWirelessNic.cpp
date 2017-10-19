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

#include <cassert>

#include "inet/common/INETDefs.h"
#include "inet/linklayer/base/MACProtocolBase.h"

#include "RangingWirelessNic.h"

namespace smile {

Define_Module(RangingWirelessNic);

const omnetpp::simsignal_t RangingWirelessNic::transmissionCompletedSignal{
    omnetpp::cComponent::registerSignal("transmissionCompleted")};
const omnetpp::simsignal_t RangingWirelessNic::receptionCompletedSignal{
    omnetpp::cComponent::registerSignal("receptionCompletedSignal")};

RangingWirelessNic::RangingWirelessNic() :
    startScheduleOperationMessage{std::make_unique<omnetpp::cMessage>("Start scheduled operation")}
{}

RangingWirelessNic::~RangingWirelessNic()
{
  cancelEvent(startScheduleOperationMessage.get());
}

const inet::MACAddress &RangingWirelessNic::getMacAddress() const
{
  return address;
}

bool RangingWirelessNic::scheduleTransmission(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime &delay,
                                              bool cancelScheduledOperation)
{
  return scheduleOperation(Operation::Type::TX, std::move(frame), delay, cancelScheduledOperation);
}

bool RangingWirelessNic::scheduleReception(const omnetpp::SimTime &delay, bool cancelScheduledOperation)
{
  return scheduleOperation(Operation::Type::RX, nullptr, delay, cancelScheduledOperation);
}

void RangingWirelessNic::initialize(int stage)
{
  cSimpleModule::initialize(stage);

  if (stage == inet::INITSTAGE_PHYSICAL_ENVIRONMENT_2) {
    initializeClock();
    initializeRadio();
    initializeMac();
    initializeMobility();
  }
}

void RangingWirelessNic::receiveSignal(omnetpp::cComponent *, omnetpp::simsignal_t signalID, long value,
                                       omnetpp::cObject *)
{
  if (signalID == inet::physicallayer::Radio::transmissionStateChangedSignal) {
    handleTransmissionStateChangedSignal(static_cast<inet::physicallayer::IRadio::TransmissionState>(value));
  }
  else if (signalID == inet::physicallayer::Radio::receptionStateChangedSignal) {
    handleReceptionStateChangedSignal(static_cast<inet::physicallayer::IRadio::ReceptionState>(value));
  }
  else {
    throw omnetpp::cRuntimeError{"Received unexpected signal: \"%s\"", getSignalName(signalID)};
  }
}

void RangingWirelessNic::receiveSignal(omnetpp::cComponent *, omnetpp::simsignal_t signalID,
                                       const omnetpp::SimTime &value, omnetpp::cObject *)
{
  if (signalID == IClock::windowUpdateSignal) {
    handleWindowUpdateSignal(value);
  }
  else {
    throw omnetpp::cRuntimeError{"Received unexpected signal: \"%s\"", getSignalName(signalID)};
  }
}

void RangingWirelessNic::handleTransmissionStateChangedSignal(inet::physicallayer::IRadio::TransmissionState newState)
{
  switch (newState) {
    case inet::physicallayer::IRadio::TRANSMISSION_STATE_TRANSMITTING:
      lastTxOperation.set(clock->getClockTimestamp());
      EV_DETAIL << "Transmission started at " << lastTxOperation.getTimestamp() << endl;
      break;
    case inet::physicallayer::IRadio::TRANSMISSION_STATE_IDLE:
      if (radio->getTransmissionState() == inet::physicallayer::IRadio::TRANSMISSION_STATE_TRANSMITTING) {
        handleTransmisionCompletion();
        EV_DETAIL << "Transmission finished at " << clock->getClockTimestamp() << endl;
      }
      break;
    case inet::physicallayer::IRadio::TRANSMISSION_STATE_UNDEFINED:
      lastTxOperation.clear();
      break;
  }
}

void RangingWirelessNic::handleReceptionStateChangedSignal(inet::physicallayer::IRadio::ReceptionState newState)
{
  switch (newState) {
    case inet::physicallayer::IRadio::RECEPTION_STATE_RECEIVING:
      lastRxOperation.set(clock->getClockTimestamp());
      EV_DETAIL << "Reception started at " << lastTxOperation.getTimestamp() << endl;
      break;
    case inet::physicallayer::IRadio::RECEPTION_STATE_BUSY:
      lastRxOperation.set(omnetpp::SimTime{0});
      break;
    case inet::physicallayer::IRadio::RECEPTION_STATE_IDLE:
      if (radio->getReceptionState() == inet::physicallayer::IRadio::RECEPTION_STATE_RECEIVING) {
        handleReceptionCompletion();
        EV_DETAIL << "Reception finished at " << clock->getClockTimestamp() << endl;
      }
      break;
    case inet::physicallayer::IRadio::RECEPTION_STATE_UNDEFINED:
      lastRxOperation.clear();
      break;
  }
}

bool RangingWirelessNic::scheduleOperation(Operation::Type type, std::unique_ptr<inet::MACFrameBase> frame,
                                           const omnetpp::SimTime &delay, bool cancelScheduledOperation)
{
  if (radio->getRadioMode() != inet::physicallayer::Radio::RADIO_MODE_OFF ||
      radio->getRadioMode() != inet::physicallayer::Radio::RADIO_MODE_SLEEP) {
    return false;
  }

  if (!cancelScheduledOperation && scheduledOperation) {
    return false;
  }

  const auto clockTimestamp = clock->getClockTimestamp() + delay;
  scheduledOperation.set(type, std::move(frame), clockTimestamp);

  const auto simulationTimestamp = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTimestamp) {
    scheduleAt(*simulationTimestamp, startScheduleOperationMessage.get());
  }
  else {
    radio->subscribe(IClock::windowUpdateSignal, this);
  }

  EV_DETAIL << scheduledOperation.getTypeAsString() << " operation was scheduled" << endl;

  return true;
}

void RangingWirelessNic::handleTransmisionCompletion()
{
  assert(lastTxOperation);
  auto frameTuple = lastTxOperation.release();
  emit(transmissionCompletedSignal, frameTuple.release());

  EV_DETAIL << "TX operation was completed" << endl;
}

void RangingWirelessNic::handleReceptionCompletion()
{
  assert(lastRxOperation);
  auto frameTuple = lastRxOperation.release();
  emit(receptionCompletedSignal, frameTuple.release());

  EV_DETAIL << "RX operation was completed" << endl;
}

void RangingWirelessNic::handleStartScheduleOperationMessage()
{
  const auto mode = scheduledOperation.getRadioMode();
  radio->setRadioMode(mode);

  EV_DETAIL << "Enable NIC's radio for scheduled " << scheduledOperation.getTypeAsString() << " operation" << endl;
}

void RangingWirelessNic::handleWindowUpdateSignal(const omnetpp::SimTime &windowEndClockTimestamp)
{
  bool releaseSubscription{true};
  if (scheduledOperation) {
    scheduleAt(0, startScheduleOperationMessage.get());
  }

  if (releaseSubscription) {
    unsubscribe(IClock::windowUpdateSignal, this);
  }
}

int RangingWirelessNic::numInitStages() const
{
  return inet::INITSTAGE_LINK_LAYER_2 + 1;
}

void RangingWirelessNic::handleMessage(omnetpp::cMessage *message)
{
  if (message->isSelfMessage()) {
    if (message == startScheduleOperationMessage.get()) {
      handleStartScheduleOperationMessage();
    }
    else {
      throw omnetpp::cRuntimeError{"Received unknown self message: \"%s\"", message->getFullName()};
    }
  }
  else {
    std::unique_ptr<omnetpp::cMessage>{message};
    // TODO
  }
}

void RangingWirelessNic::initializeRadio()
{
  radio = check_and_cast<inet::physicallayer::Radio *>(getModuleByPath(".nic.radio"));
  radio->subscribe(inet::physicallayer::Radio::transmissionStateChangedSignal, this);
  radio->subscribe(inet::physicallayer::Radio::receptionStateChangedSignal, this);

  const auto enableReceiverOnStart = par("enableReceiverOnStart").boolValue();
  if(enableReceiverOnStart) {
      scheduleReception(0);
  }
}

void RangingWirelessNic::initializeMac()
{
  mac = check_and_cast<inet::IdealMac *>(getModuleByPath(".nic.mac"));
  address.setAddress(mac->par("address").stringValue());
}

void RangingWirelessNic::initializeClock()
{
  clock = check_and_cast<Clock *>(getModuleByPath(".clock"));
}

void RangingWirelessNic::initializeMobility()
{
  auto mobility = check_and_cast<omnetpp::cComponent *>(getModuleByPath(".mobility"));
  mobility->subscribe(inet::IMobility::mobilityStateChangedSignal, this);

  auto iMobility = check_and_cast<inet::IMobility *>(mobility);
  currentPosition = iMobility->getCurrentPosition();
  EV_DETAIL << "Current position: " << currentPosition << endl;
}

RangingWirelessNic::Operation::operator bool() const
{
  return frame && timestamp > 0;
}

void RangingWirelessNic::Operation::set(std::unique_ptr<inet::MACFrameBase> newFrame)
{
  frame = std::move(newFrame);
}

void RangingWirelessNic::Operation::set(const omnetpp::SimTime &newTimestamp)
{
  timestamp = newTimestamp;
}

void RangingWirelessNic::Operation::set(Operation::Type newType, std::unique_ptr<inet::MACFrameBase> newFrame,
                                        const omnetpp::SimTime &newTimestamp)
{
  type = newType;
  set(std::move(newFrame));
  set(newTimestamp);
}

RangingWirelessNic::Operation::Type RangingWirelessNic::Operation::getType() const
{
  return type;
}

const char *RangingWirelessNic::Operation::getTypeAsString() const
{
  {
    switch (type) {
      case Operation::Type::TX:
        return "TX";
      case Operation::Type::RX:
        return "RX";
      default:
        throw omnetpp::cRuntimeError("Cannot convert unknown RangingWirelessNic::Operation::Type (int) %x to string",
                                     static_cast<int>(type));
    }
  }
}

inet::physicallayer::Radio::RadioMode RangingWirelessNic::Operation::getRadioMode() const
{
  switch (type) {
    case Operation::Type::TX:
      return inet::physicallayer::Radio::RADIO_MODE_TRANSMITTER;
    case Operation::Type::RX:
      return inet::physicallayer::Radio::RADIO_MODE_RECEIVER;
    default:
      throw omnetpp::cRuntimeError(
          "Cannot cast unknown RangingWirelessNic::Operation::Type (int) %x to inet::physicallayer::Radio::RadioMode",
          static_cast<int>(type));
  }
}

const std::unique_ptr<inet::MACFrameBase> &RangingWirelessNic::Operation::getFrame() const
{
  return frame;
}

const omnetpp::SimTime &RangingWirelessNic::Operation::getTimestamp() const
{
  return timestamp;
}

std::unique_ptr<FrameTuple> RangingWirelessNic::Operation::release()
{
  auto result = std::make_unique<FrameTuple>(std::move(frame), timestamp);
  clear();
  return result;
}

void RangingWirelessNic::Operation::clear()
{
  set(Type::TX, nullptr, 0);
}

}  // namespace smile
