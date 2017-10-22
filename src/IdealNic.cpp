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

IdealNic::IdealNic() : startScheduleOperationMessage{std::make_unique<omnetpp::cMessage>("Start scheduled operation")}
{}

IdealNic::~IdealNic()
{
  cancelEvent(startScheduleOperationMessage.get());
}

bool IdealNic::configureDelayedTransmission(const omnetpp::SimTime &delay, bool cancelScheduledOperation)
{
  return scheduleOperation(inet::physicallayer::IRadio::RADIO_MODE_TRANSCEIVER, delay, cancelScheduledOperation);
}

bool IdealNic::configureDelayedReception(const omnetpp::SimTime &delay, bool cancelScheduledOperation)
{
  return scheduleOperation(inet::physicallayer::IRadio::RADIO_MODE_RECEIVER, delay, cancelScheduledOperation);
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
  scheduledOperation.frame = std::move(frame);
}

void IdealNic::initializeRadio()
{
  using inet::physicallayer::IRadio;
  radio = check_and_cast<inet::physicallayer::Radio *>(getModuleByPath(".nic.radio"));
  radio->subscribe(IRadio::transmissionStateChangedSignal, this);
  radio->subscribe(IRadio::receptionStateChangedSignal, this);
  radio->subscribe(IRadio::radioModeChangedSignal, this);

  const auto enableReceiverOnStart = par("enableReceiverOnStart").boolValue();
  if (enableReceiverOnStart) {
    configureDelayedReception(0, true);
  }
}

void IdealNic::initializeMac()
{
  mac = check_and_cast<inet::IdealMac *>(getModuleByPath(".nic.mac"));
  setAddress(inet::MACAddress{mac->par("address").stringValue()});
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

void IdealNic::handleTransmissionStateChangedSignal(inet::physicallayer::IRadio::TransmissionState newState)
{
  using inet::physicallayer::IRadio;
  switch (newState) {
    case inet::physicallayer::IRadio::TRANSMISSION_STATE_TRANSMITTING:
      setTransmissionBeginClockTimestamp(clock->getClockTimestamp());
      EV_DETAIL << "Transmission started at " << getTransmissionBeginClockTimestamp() << endl;
      break;
    case inet::physicallayer::IRadio::TRANSMISSION_STATE_IDLE:
      if (radio->getTransmissionState() == IRadio::TRANSMISSION_STATE_TRANSMITTING) {
        emit(transmissionCompletedSignal, scheduledOperation.frame->dup());
        radio->setRadioMode(IRadio::RADIO_MODE_OFF);
        EV_DETAIL << "Transmission finished at " << clock->getClockTimestamp() << endl;
      }
      break;
    default:
      break;  // Ignore
  }
}

void IdealNic::handleReceptionStateChangedSignal(inet::physicallayer::IRadio::ReceptionState newState)
{
  using inet::physicallayer::IRadio;
  switch (newState) {
    case IRadio::RECEPTION_STATE_RECEIVING:
      setReceptionBeginClockTimestamp(clock->getClockTimestamp());
      EV_DETAIL << "Reception started at " << getReceptionBeginClockTimestamp() << endl;
      break;
    case IRadio::RECEPTION_STATE_IDLE:
      if (radio->getReceptionState() == IRadio::RECEPTION_STATE_RECEIVING) {
        radio->setRadioMode(IRadio::RADIO_MODE_OFF);
        EV_DETAIL << "Reception finished at " << clock->getClockTimestamp() << endl;
      }
      break;
    default:
      break;  // Ignore
  }
}

void IdealNic::handleradioModeChangedSignal(inet::physicallayer::IRadio::RadioMode newMode)
{
  using inet::physicallayer::IRadio;
  if (newMode == IRadio::RADIO_MODE_OFF) {
    switch (scheduledOperation.radioMode) {
      case IRadio::RADIO_MODE_TRANSMITTER:
        emit(transmissionCompletedSignal, scheduledOperation.frame.release());
        break;
      case IRadio::RADIO_MODE_RECEIVER:
        send(scheduledOperation.frame.release(), "upperLayerIn");
        break;
      default:
        break;  // TODO
    }

    scheduledOperation.clear();
  }
  else if (newMode == IRadio::RADIO_MODE_TRANSMITTER) {
    send(scheduledOperation.frame->dup(), "lowerLayerIn");
  }
  else {
    // Ignore other modes
  }
}

void IdealNic::handleStartScheduleOperationMessage()
{
  radio->setRadioMode(scheduledOperation.radioMode);
  EV_DETAIL << "Enable NIC's radio for scheduled operation" << endl;
}

void IdealNic::handleWindowUpdateSignal(const omnetpp::SimTime &)
{
  auto releaseSubscription{true};
  if (scheduledOperation) {
    const auto simulationTimestamp = clock->convertToSimulationTimestamp(scheduledOperation.beginClockTimestamp);
    if(simulationTimestamp){
      scheduleAt(*simulationTimestamp, startScheduleOperationMessage.get());
    }
  }

  if (releaseSubscription) {
    unsubscribe(IClock::windowUpdateSignal, this);
  }
}

bool IdealNic::scheduleOperation(inet::physicallayer::IRadio::RadioMode mode, const omnetpp::SimTime &delay,
                                 bool cancelScheduledOperation)
{
  using inet::physicallayer::IRadio;

  if (radio->getRadioMode() != IRadio::RADIO_MODE_OFF) {
    return false;
  }

  if (!cancelScheduledOperation && scheduledOperation) {
    return false;
  }

  if (mode != IRadio::RADIO_MODE_TRANSCEIVER && mode != IRadio::RADIO_MODE_RECEIVER) {
    throw omnetpp::cRuntimeError{
        "Cannot schedule (inet::physicallayer::IRadio::RadioMode) 0x%x operation, only RADIO_MODE_TRANSCEIVER and "
        "RADIO_MODE_RECEIVER are allowed",
        mode};
  }

  scheduledOperation.clear();
  scheduledOperation.radioMode = mode;
  scheduledOperation.beginClockTimestamp = clock->getClockTimestamp() + delay;

  const auto simulationTimestamp = clock->convertToSimulationTimestamp(scheduledOperation.beginClockTimestamp);
  if (simulationTimestamp) {
    scheduleAt(*simulationTimestamp, startScheduleOperationMessage.get());
  }
  else {
    radio->subscribe(IClock::windowUpdateSignal, this);
  }

  const auto operationType = mode == IRadio::RADIO_MODE_TRANSCEIVER ? "TX" : "RX";
  EV_DETAIL << "Schedule " << operationType << " operation at " << scheduledOperation.beginClockTimestamp
            << " (clock time)" << endl;

  return true;
}

IdealNic::Operation::operator bool() const
{
  return frame && (beginClockTimestamp > 0);
}

void IdealNic::Operation::clear()
{
  radioMode = inet::physicallayer::IRadio::RADIO_MODE_SWITCHING;
  frame = nullptr;
  beginClockTimestamp = 0;
}

}  // namespace smile
