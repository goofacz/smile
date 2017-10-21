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

#include "IdealNic.h"

#include <cassert>

#include "inet/common/INETDefs.h"
#include "inet/linklayer/base/MACProtocolBase.h"

namespace smile {

Define_Module(IdealNic);

const omnetpp::simsignal_t IdealNic::transmissionCompletedSignal{
    omnetpp::cComponent::registerSignal("transmissionCompleted")};
const omnetpp::simsignal_t IdealNic::receptionCompletedSignal{
    omnetpp::cComponent::registerSignal("receptionCompletedSignal")};

IdealNic::IdealNic() : startScheduleOperationMessage{std::make_unique<omnetpp::cMessage>("Start scheduled operation")}
{}

IdealNic::~IdealNic()
{
  cancelEvent(startScheduleOperationMessage.get());
}

const inet::MACAddress &IdealNic::getMacAddress() const
{
  return address;
}

bool IdealNic::scheduleTransmission(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime &delay,
                                    bool cancelScheduledOperation)
{
  return scheduleOperation(Operation::Type::TX, std::move(frame), delay, cancelScheduledOperation);
}

bool IdealNic::scheduleReception(const omnetpp::SimTime &delay, bool cancelScheduledOperation)
{
  return scheduleOperation(Operation::Type::RX, nullptr, delay, cancelScheduledOperation);
}

void IdealNic::initialize(int stage)
{
  cSimpleModule::initialize(stage);

  if (stage == inet::INITSTAGE_PHYSICAL_ENVIRONMENT_2) {
    initializeClock();
    initializeRadio();
    initializeMac();
    initializeMobility();
  }
}

void IdealNic::receiveSignal(omnetpp::cComponent *, omnetpp::simsignal_t signalID, long value, omnetpp::cObject *)
{
  if (signalID == inet::physicallayer::IRadio::transmissionStateChangedSignal) {
    handleTransmissionStateChangedSignal(static_cast<inet::physicallayer::IRadio::TransmissionState>(value));
  }
  else if (signalID == inet::physicallayer::IRadio::receptionStateChangedSignal) {
    handleReceptionStateChangedSignal(static_cast<inet::physicallayer::IRadio::ReceptionState>(value));
  }
  else if (signalID == inet::physicallayer::IRadio::radioModeChangedSignal) {
    handleradioModeChangedSignal(static_cast<inet::physicallayer::IRadio::RadioMode>(value));
  }
  else {
    throw omnetpp::cRuntimeError{"Received unexpected signal: \"%s\"", getSignalName(signalID)};
  }
}

void IdealNic::receiveSignal(omnetpp::cComponent *, omnetpp::simsignal_t signalID, const omnetpp::SimTime &value,
                             omnetpp::cObject *)
{
  if (signalID == IClock::windowUpdateSignal) {
    handleWindowUpdateSignal(value);
  }
  else {
    throw omnetpp::cRuntimeError{"Received unexpected signal: \"%s\"", getSignalName(signalID)};
  }
}

void IdealNic::handleTransmissionStateChangedSignal(inet::physicallayer::IRadio::TransmissionState newState)
{
  switch (newState) {
    case inet::physicallayer::IRadio::TRANSMISSION_STATE_TRANSMITTING:
      lastTxOperation.set(clock->getClockTimestamp());
      EV_DETAIL << "Transmission started at " << lastTxOperation.getTimestamp() << endl;
      break;
    case inet::physicallayer::IRadio::TRANSMISSION_STATE_IDLE:
      if (radio->getTransmissionState() == inet::physicallayer::IRadio::TRANSMISSION_STATE_TRANSMITTING) {
        handleTransmisionCompletion();
        radio->setRadioMode(inet::physicallayer::IRadio::RADIO_MODE_OFF);
        EV_DETAIL << "Transmission finished at " << clock->getClockTimestamp() << endl;
      }
      break;
    case inet::physicallayer::IRadio::TRANSMISSION_STATE_UNDEFINED:
      lastTxOperation.clear();
      break;
  }
}

void IdealNic::handleReceptionStateChangedSignal(inet::physicallayer::IRadio::ReceptionState newState)
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
        radio->setRadioMode(inet::physicallayer::IRadio::RADIO_MODE_OFF);
        EV_DETAIL << "Reception finished at " << clock->getClockTimestamp() << endl;
      }
      break;
    case inet::physicallayer::IRadio::RECEPTION_STATE_UNDEFINED:
      lastRxOperation.clear();
      break;
  }
}

void IdealNic::handleradioModeChangedSignal(inet::physicallayer::IRadio::RadioMode newMode)
{
  if (radio->getRadioMode() != inet::physicallayer::IRadio::RADIO_MODE_OFF) {
    return;
  }

  if (!scheduledOperation) {
    throw omnetpp::cRuntimeError{"RangingWirelessNic failed to find scheduled operation after switching to TX/RX"};
  }

  const auto operationType = scheduledOperation.getType();
  if (operationType == Operation::Type::TX) {
    if (newMode != inet::physicallayer::IRadio::RADIO_MODE_TRANSMITTER) {
      throw omnetpp::cRuntimeError{
          "RangingWirelessNic has scheduled TX operation, but radio switched to different mode "
          "(inet::physicallayer::IRadio): 0x%x",
          static_cast<int>(newMode)};
    }

    lastTxOperation = std::move(scheduledOperation);
    lastTxOperation.set(omnetpp::SimTime{});

    const auto &frame = lastTxOperation.getFrame();
    sendDelayed(frame->dup(), 0, "lowerLayerIn");
  }
  else if (operationType == Operation::Type::RX) {
    if (newMode != inet::physicallayer::IRadio::RADIO_MODE_RECEIVER) {
      throw omnetpp::cRuntimeError{
          "RangingWirelessNic has scheduled RX operation, but radio switched to different mode "
          "(inet::physicallayer::IRadio): 0x%x",
          static_cast<int>(newMode)};
    }

    lastRxOperation.clear();
  }
  else {
    throw omnetpp::cRuntimeError{
        "RangingWirelessNic encountered invalid type of scheduled operation (Operation::Type): 0x%x", operationType};
  }

  scheduledOperation.clear();
}

bool IdealNic::scheduleOperation(Operation::Type type, std::unique_ptr<inet::MACFrameBase> frame,
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

void IdealNic::handleTransmisionCompletion()
{
  assert(lastTxOperation);
  auto frameTuple = lastTxOperation.release();
  emit(transmissionCompletedSignal, frameTuple.release());

  EV_DETAIL << "TX operation was completed" << endl;
}

void IdealNic::handleReceptionCompletion()
{
  assert(lastRxOperation);
  auto frameTuple = lastRxOperation.release();
  emit(receptionCompletedSignal, frameTuple.release());

  EV_DETAIL << "RX operation was completed" << endl;
}

void IdealNic::handleStartScheduleOperationMessage()
{
  const auto mode = scheduledOperation.getRadioMode();
  radio->setRadioMode(mode);

  EV_DETAIL << "Enable NIC's radio for scheduled " << scheduledOperation.getTypeAsString() << " operation" << endl;
}

void IdealNic::handleWindowUpdateSignal(const omnetpp::SimTime &windowEndClockTimestamp)
{
  bool releaseSubscription{true};
  if (scheduledOperation) {
    scheduleAt(0, startScheduleOperationMessage.get());
  }

  if (releaseSubscription) {
    unsubscribe(IClock::windowUpdateSignal, this);
  }
}

int IdealNic::numInitStages() const
{
  return inet::INITSTAGE_LINK_LAYER_2 + 1;
}

void IdealNic::handleMessage(omnetpp::cMessage *message)
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
    if (message->getArrivalGate() == gate("lowerLayerIn")) {
      std::unique_ptr<inet::MACFrameBase> frame{check_and_cast<inet::MACFrameBase *>(message)};
      handleReceivedFrame(std::move(frame));
    }
  }
}

void IdealNic::handleReceivedFrame(std::unique_ptr<inet::MACFrameBase> frame)
{
  lastRxOperation.set(std::move(frame));

  assert(lastTxOperation);
  auto frameTuple = lastRxOperation.release();
  emit(receptionCompletedSignal, frameTuple.release());
}

void IdealNic::initializeRadio()
{
  radio = check_and_cast<inet::physicallayer::Radio *>(getModuleByPath(".nic.radio"));
  radio->subscribe(inet::physicallayer::IRadio::transmissionStateChangedSignal, this);
  radio->subscribe(inet::physicallayer::IRadio::receptionStateChangedSignal, this);
  radio->subscribe(inet::physicallayer::IRadio::radioModeChangedSignal, this);

  const auto enableReceiverOnStart = par("enableReceiverOnStart").boolValue();
  if (enableReceiverOnStart) {
    scheduleReception(0);
  }
}

void IdealNic::initializeMac()
{
  mac = check_and_cast<inet::IdealMac *>(getModuleByPath(".nic.mac"));
  address.setAddress(mac->par("address").stringValue());
}

void IdealNic::initializeClock()
{
  clock = check_and_cast<Clock *>(getModuleByPath(".clock"));
}

void IdealNic::initializeMobility()
{
  auto mobility = check_and_cast<omnetpp::cComponent *>(getModuleByPath(".mobility"));
  mobility->subscribe(inet::IMobility::mobilityStateChangedSignal, this);

  auto iMobility = check_and_cast<inet::IMobility *>(mobility);
  currentPosition = iMobility->getCurrentPosition();
  EV_DETAIL << "Current position: " << currentPosition << endl;
}

IdealNic::Operation::operator bool() const
{
  return frame && timestamp > 0;
}

void IdealNic::Operation::set(std::unique_ptr<inet::MACFrameBase> newFrame)
{
  frame = std::move(newFrame);
}

void IdealNic::Operation::set(const omnetpp::SimTime &newTimestamp)
{
  timestamp = newTimestamp;
}

void IdealNic::Operation::set(Operation::Type newType, std::unique_ptr<inet::MACFrameBase> newFrame,
                              const omnetpp::SimTime &newTimestamp)
{
  type = newType;
  set(std::move(newFrame));
  set(newTimestamp);
}

IdealNic::Operation::Type IdealNic::Operation::getType() const
{
  return type;
}

const char *IdealNic::Operation::getTypeAsString() const
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

inet::physicallayer::Radio::RadioMode IdealNic::Operation::getRadioMode() const
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

const std::unique_ptr<inet::MACFrameBase> &IdealNic::Operation::getFrame() const
{
  return frame;
}

const omnetpp::SimTime &IdealNic::Operation::getTimestamp() const
{
  return timestamp;
}

std::unique_ptr<FrameTuple> IdealNic::Operation::release()
{
  auto result = std::make_unique<FrameTuple>(std::move(frame), timestamp);
  clear();
  return result;
}

void IdealNic::Operation::clear()
{
  set(Type::TX, nullptr, 0);
}

}  // namespace smile
