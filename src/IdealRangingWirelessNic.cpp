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

inet::MACAddress IdealRangingWirelessNic::getMacAddress() const
{
  return inet::MACAddress{mac->par("address").stringValue()};
}

void IdealRangingWirelessNic::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    const auto enableIdealInterface = par("enableIdealInterface").boolValue();
    if (!enableIdealInterface) {
      throw cRuntimeError{"IdealRangingWirelessNic requires enableIdealInterface to be set"};
    }

    const auto nicModulePath = ".nic";
    nic = getModuleByPath(nicModulePath);
    if (!nic) {
      throw cRuntimeError{"Failed to find \"%s\" module", nicModulePath};
    }

    const auto radioModulePath = ".nic.radio";
    auto radioModule = getModuleByPath(radioModulePath);
    if (!radioModule) {
      throw cRuntimeError{"Failed to find \"%s\" module", radioModulePath};
    }
    radio = check_and_cast<inet::physicallayer::IRadio*>(radioModule);
    radioModule->subscribe(inet::physicallayer::IRadio::transmissionStateChangedSignal, this);
    radioModule->subscribe(inet::physicallayer::IRadio::receptionStateChangedSignal, this);

    const auto macModulePath = ".nic.mac";
    mac = getModuleByPath(macModulePath);
    if (!mac) {
      throw cRuntimeError{"Failed to find \"%s\" module", macModulePath};
    }
  }
}

void IdealRangingWirelessNic::handleIncommingMessage(omnetpp::cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  const auto arrivalGate = message->getArrivalGate();
  if (arrivalGate == gate("nicIn")) {
    handleNicIn(dynamic_unique_ptr_cast<inet::IdealMacFrame>(std::move(message)));
  }
  else if (arrivalGate == gate("upperLayerIn")) {
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
  txFrame.reset(frame->dup());
  txCompletion.setFrame(txFrame.get());
  send(frame.release(), "nicOut");
}

void IdealRangingWirelessNic::handleNicIn(std::unique_ptr<inet::IdealMacFrame> frame)
{
  rxFrame.reset(frame->dup());
  rxCompletion.setFrame(rxFrame.get());

  EV_DETAIL_C("IdealRangingWirelessNic") << "Frame " << rxCompletion.getFrame()->getClassName()
                                         << " (ID: " << rxCompletion.getFrame()->getId() << ") reception completed at "
                                         << clockTime() << "(local clock)" << endl;

  emit(IRangingWirelessNic::rxCompletedSignalId, &rxCompletion);
  send(frame.release(), "upperLayerIn");
}

void IdealRangingWirelessNic::handleRadioStateChanged(inet::physicallayer::IRadio::TransmissionState newState)
{
  using inet::physicallayer::IRadio;
  switch (newState) {
    case IRadio::TRANSMISSION_STATE_IDLE:
      EV_DETAIL_C("IdealRangingWirelessNic")
          << "Frame " << txCompletion.getFrame()->getClassName() << " (ID: " << txCompletion.getFrame()->getId()
          << ") transmission completed at " << clockTime() << "(local clock)" << endl;
      emit(IRangingWirelessNic::txCompletedSignalId, &txCompletion);
      break;
    case IRadio::TRANSMISSION_STATE_TRANSMITTING:
      EV_DETAIL_C("IdealRangingWirelessNic")
          << "Frame " << txCompletion.getFrame()->getClassName() << " (ID: " << txCompletion.getFrame()->getId()
          << ") transmission started at " << clockTime() << "(local clock)" << endl;
      txCompletion.setOperationBeginClockTimestamp(clockTime());
      break;
    case IRadio::TRANSMISSION_STATE_UNDEFINED:
      clearTxCompletion();
      break;
  }
}

void IdealRangingWirelessNic::handleRadioStateChanged(inet::physicallayer::IRadio::ReceptionState newState)
{
  using inet::physicallayer::IRadio;
  switch (newState) {
    case IRadio::RECEPTION_STATE_BUSY:
      // TODO
      break;
    case IRadio::RECEPTION_STATE_IDLE:
      break;
    case IRadio::RECEPTION_STATE_RECEIVING:
      EV_DETAIL_C("IdealRangingWirelessNic")
          << "Frame " << rxCompletion.getFrame()->getClassName() << " (ID: " << rxCompletion.getFrame()->getId()
          << ") reception started at " << clockTime() << "(local clock)" << endl;
      rxCompletion.setOperationBeginClockTimestamp(clockTime());
      break;
    case IRadio::RECEPTION_STATE_UNDEFINED:
      clearRxCompletion();
      break;
  }
}

void IdealRangingWirelessNic::clearRxCompletion()
{
  txCompletion.setFrame(nullptr);
  txCompletion.setOperationBeginClockTimestamp(0);
  txCompletion.setStatus(IdealTxCompletionStatus::SUCCESS);
  txFrame.reset();
}

void IdealRangingWirelessNic::clearTxCompletion()
{
  rxCompletion.setFrame(nullptr);
  rxCompletion.setOperationBeginClockTimestamp(0);
  rxCompletion.setStatus(IdealRxCompletionStatus::SUCCESS);
  rxFrame.reset();
}

}  // namespace smile
