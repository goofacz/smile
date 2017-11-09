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

#include "IdealRangingWirelessNic.h"
#include "utilities.h"

namespace smile {

Define_Module(IdealRangingWirelessNic);

void IdealRangingWirelessNic::initialize(int stage)
{
  ClockDecorator<omnetpp::cSimpleModule>::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    const auto enableIdealInterface = par("enableIdealInterface").boolValue();
    if (!enableIdealInterface) {
      throw cRuntimeError{"IdealRangingWirelessNic requires enableIdealInterface to be set"};
    }

    auto radioModule = getModuleByPath("nic.radio");
    if (!radioModule) {
      throw cRuntimeError{"Failed to find \"nic.radio\" module"};
    }
    radio = check_and_cast<inet::physicallayer::IRadio*>(radioModule);
    radioModule->subscribe(inet::physicallayer::IRadio::transmissionStateChangedSignal, this);
    radioModule->subscribe(inet::physicallayer::IRadio::receptionStateChangedSignal, this);
  }
}

void IdealRangingWirelessNic::handleIncommingMessage(omnetpp::cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  const auto arrivalGate = message->getArrivalGate();
  if (arrivalGate == gate("nicIn")) {
    handleNicIn(dynamic_unique_ptr_cast<inet::IdealMacFrame>(std::move(message)));
  }
  else if (arrivalGate == gate("idealIn")) {
    handleIdealIn(dynamic_unique_ptr_cast<inet::IdealMacFrame>(std::move(message)));
  }
  else {
    throw cRuntimeError{"Received unexpected message \"%s\" on gate \"%s\"", message->getFullName(),
                        arrivalGate->getFullName()};
  }
}

void IdealRangingWirelessNic::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                                            omnetpp::cObject* details)
{
  if (signalID == inet::physicallayer::IRadio::transmissionStateChangedSignal) {
    handleRadioStateChanged(static_cast<inet::physicallayer::IRadio::TransmissionState>(value));
  }
  else if (signalID == inet::physicallayer::IRadio::receptionStateChangedSignal) {
    handleRadioStateChanged(static_cast<inet::physicallayer::IRadio::ReceptionState>(value));
  }
}

void IdealRangingWirelessNic::handleIdealIn(std::unique_ptr<inet::IdealMacFrame> frame)
{
  send(frame.release(), "nicIn");
}

void IdealRangingWirelessNic::handleNicIn(std::unique_ptr<inet::IdealMacFrame> frame)
{
  emit(IRangingWirelessNic::rxCompletedSignalId, &rxCompletion);
  send(frame.release(), "idealIn");
}

void IdealRangingWirelessNic::handleRadioStateChanged(inet::physicallayer::IRadio::TransmissionState newState)
{
  using inet::physicallayer::IRadio;
  switch (newState) {
    case IRadio::TRANSMISSION_STATE_IDLE:
      emit(IRangingWirelessNic::txCompletedSignalId, &txCompletion);
      break;
    case IRadio::TRANSMISSION_STATE_TRANSMITTING:
      txCompletion.setOperationBeginClockTimestamp(clockTime());
      break;
    case IRadio::TRANSMISSION_STATE_UNDEFINED:
      break;
  }
}

void IdealRangingWirelessNic::handleRadioStateChanged(inet::physicallayer::IRadio::ReceptionState newState)
{
  using inet::physicallayer::IRadio;
  switch (newState) {
    case IRadio::RECEPTION_STATE_BUSY:
      break;
    case IRadio::RECEPTION_STATE_IDLE:
      break;
    case IRadio::RECEPTION_STATE_RECEIVING:
      rxCompletion.setOperationBeginClockTimestamp(clockTime());
      break;
    case IRadio::RECEPTION_STATE_UNDEFINED:
      break;
  }
}

}  // namespace smile
