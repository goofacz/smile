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

#include "IdealRangingNicDriver.h"
#include "utilities.h"

namespace smile {

Define_Module(IdealRangingNicDriver);

inet::MACAddress IdealRangingNicDriver::getMacAddress() const
{
  return inet::MACAddress{mac->par("address").stringValue()};
}

void IdealRangingNicDriver::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    const auto nicModulePath = par("nicModuleRelativePath").stringValue();
    nic = getModuleByPath(nicModulePath);
    if (!nic) {
      throw cRuntimeError{"Failed to find \"%s\" module", nicModulePath};
    }

    const auto radioModulePath = ".radio";
    auto radioModule = nic->getModuleByPath(radioModulePath);
    if (!radioModule) {
      throw cRuntimeError{"Failed to find \"%s\" module relative to \"nic\" module", radioModulePath};
    }

    radio = check_and_cast<inet::physicallayer::IRadio*>(radioModule);
    radioModule->subscribe(inet::physicallayer::IRadio::transmissionStateChangedSignal, this);
    radioModule->subscribe(inet::physicallayer::IRadio::receptionStateChangedSignal, this);

    const auto macModulePath = ".mac";
    mac = nic->getModuleByPath(macModulePath);
    if (!mac) {
      throw cRuntimeError{"Failed to find \"%s\" module relative to \"nic\" module", macModulePath};
    }

    const auto mobilityPath = par("mobilityModule").stringValue();
    mobility = check_and_cast<inet::IMobility*>(getModuleByPath(mobilityPath));
  }
}

void IdealRangingNicDriver::handleIncommingMessage(omnetpp::cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  if (message->arrivedOn("nicIn")) {
    handleNicIn(dynamic_unique_ptr_cast<inet::IdealMacFrame>(std::move(message)));
  }
  else if (message->arrivedOn("applicationIn")) {
    handleApplicationIn(dynamic_unique_ptr_cast<inet::IdealMacFrame>(std::move(message)));
  }
  else {
    throw cRuntimeError{"Received unexpected message \"%s\" on gate \"%s\"", message->getFullName(),
                        message->getArrivalGate()->getFullName()};
  }
}

void IdealRangingNicDriver::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                                          omnetpp::cObject* details)
{
  if (signalID == inet::physicallayer::IRadio::transmissionStateChangedSignal) {
    handleRadioStateChanged(static_cast<inet::physicallayer::IRadio::TransmissionState>(value));
  }
  else if (signalID == inet::physicallayer::IRadio::receptionStateChangedSignal) {
    handleRadioStateChanged(static_cast<inet::physicallayer::IRadio::ReceptionState>(value));
  }
  else {
    throw cRuntimeError{"Received unexpected signal \"%s\"", getSignalName(signalID)};
  }
}

void IdealRangingNicDriver::handleApplicationIn(std::unique_ptr<inet::IdealMacFrame> frame)
{
  txFrame.reset(frame->dup());
  txCompletion.setFrame(txFrame.get());
  send(frame.release(), "nicOut");
}

void IdealRangingNicDriver::handleNicIn(std::unique_ptr<inet::IdealMacFrame> frame)
{
  rxFrame.reset(frame->dup());
  rxCompletion.setFrame(rxFrame.get());

  EV_DETAIL_C("IdealRangingNicDriver") << "Frame " << rxCompletion.getFrame()->getClassName()
                                       << " (ID: " << rxCompletion.getFrame()->getId() << ") reception completed at "
                                       << clockTime() << " (local clock)" << endl;

  ClockDecorator<cSimpleModule>::emit(IRangingNicDriver::rxCompletedSignalId, &rxCompletion);
  send(frame.release(), "applicationOut");
}

void IdealRangingNicDriver::handleRadioStateChanged(inet::physicallayer::IRadio::TransmissionState newState)
{
  using inet::physicallayer::IRadio;
  switch (newState) {
    case IRadio::TRANSMISSION_STATE_IDLE:
      if (previousTxState == IRadio::TRANSMISSION_STATE_TRANSMITTING) {
        EV_DETAIL_C("IdealRangingNicDriver")
            << "Frame " << txCompletion.getFrame()->getClassName() << " (ID: " << txCompletion.getFrame()->getId()
            << ") transmission completed at " << clockTime() << " (local clock)" << endl;

        txCompletion.setOperationEndClockTimestamp(clockTime());
        txCompletion.setOperationEndSimulationTimestamp(simTime());
        txCompletion.setOperationEndTruePosition(mobility->getCurrentPosition());

        ClockDecorator<cSimpleModule>::emit(IRangingNicDriver::txCompletedSignalId, &txCompletion);
      }
      break;
    case IRadio::TRANSMISSION_STATE_TRANSMITTING:
      EV_DETAIL_C("IdealRangingNicDriver")
          << "Frame " << txCompletion.getFrame()->getClassName() << " (ID: " << txCompletion.getFrame()->getId()
          << ") transmission started at " << clockTime() << "(local clock)" << endl;
      txCompletion.setOperationBeginClockTimestamp(clockTime());
      txCompletion.setOperationBeginSimulationTimestamp(simTime());
      txCompletion.setOperationBeginTruePosition(mobility->getCurrentPosition());
      break;
    case IRadio::TRANSMISSION_STATE_UNDEFINED:
      clearTxCompletion();
      break;
  }

  previousTxState = newState;
}

void IdealRangingNicDriver::handleRadioStateChanged(inet::physicallayer::IRadio::ReceptionState newState)
{
  using inet::physicallayer::IRadio;
  switch (newState) {
    case IRadio::RECEPTION_STATE_BUSY:
      // TODO
      break;
    case IRadio::RECEPTION_STATE_IDLE:
      if (previousRxState == IRadio::RECEPTION_STATE_RECEIVING) {
        rxCompletion.setOperationEndClockTimestamp(clockTime());
        rxCompletion.setOperationEndSimulationTimestamp(simTime());
        rxCompletion.setOperationEndTruePosition(mobility->getCurrentPosition());
      }
      break;
    case IRadio::RECEPTION_STATE_RECEIVING:
      EV_DETAIL_C("IdealRangingNicDriver") << "Frame (Transmission ID: " << radio->getReceptionInProgress()->getId()
                                           << ") reception started at " << clockTime() << "(local clock)" << endl;
      rxCompletion.setOperationBeginClockTimestamp(clockTime());
      txCompletion.setOperationBeginSimulationTimestamp(simTime());
      rxCompletion.setOperationBeginTruePosition(mobility->getCurrentPosition());
      break;
    case IRadio::RECEPTION_STATE_UNDEFINED:
      clearRxCompletion();
      break;
  }

  previousRxState = newState;
}

void IdealRangingNicDriver::clearRxCompletion()
{
  txCompletion.setStatus(IdealTxCompletionStatus::SUCCESS);
  txCompletion.setFrame(nullptr);
  txFrame.reset();

  txCompletion.setOperationBeginClockTimestamp(0);
  txCompletion.setOperationBeginSimulationTimestamp(simTime());

  txCompletion.setOperationEndClockTimestamp(0);
  txCompletion.setOperationEndSimulationTimestamp(simTime());

  txCompletion.setOperationBeginTruePosition(inet::Coord{});
  txCompletion.setOperationEndTruePosition(inet::Coord{});
}

void IdealRangingNicDriver::clearTxCompletion()
{
  rxCompletion.setStatus(IdealRxCompletionStatus::SUCCESS);
  rxCompletion.setFrame(nullptr);
  rxFrame.reset();

  rxCompletion.setOperationBeginClockTimestamp(0);
  rxCompletion.setOperationBeginSimulationTimestamp(simTime());

  rxCompletion.setOperationEndClockTimestamp(0);
  rxCompletion.setOperationEndSimulationTimestamp(simTime());

  rxCompletion.setOperationBeginTruePosition(inet::Coord{});
  rxCompletion.setOperationEndTruePosition(inet::Coord{});
}

}  // namespace smile
