//
// Copyright (C) 2018 Tomasz Jankowski <t.jankowski AT pwr.edu.pl>
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

#include "TimeMeasurementApplicationBase.h"

using namespace inet;
using namespace inet::physicallayer;
using namespace omnetpp;

namespace smile::applications {

Define_Module(TimeMeasurementApplicationBase);

void TimeMeasurementApplicationBase::initialize(int stage)
{
  ApplicationBase::initialize(stage);
  if (stage == INITSTAGE_LOCAL) {
    auto radio{getRadio()};
    txCurrentState = radio->getTransmissionState();
    rxCurrentState = radio->getReceptionState();

    auto module{omnetpp::check_and_cast<cSimpleModule*>(radio)};
    module->subscribe(IRadio::transmissionStateChangedSignal, this);
    module->subscribe(IRadio::receptionStateChangedSignal, this);
  }
}

void TimeMeasurementApplicationBase::onTxOperationBegin(const ITransmission* transmision)
{
  // nop
}

void TimeMeasurementApplicationBase::onRxOperationBegin(const ITransmission* reception)
{
  // nop
}

void TimeMeasurementApplicationBase::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID,
                                                   long value, omnetpp::cObject* details)
{
  auto radio{getRadio()};
  if (signalID == IRadio::transmissionStateChangedSignal) {
    if (txCurrentState == IRadio::TransmissionState::TRANSMISSION_STATE_IDLE &&
        value == IRadio::TransmissionState::TRANSMISSION_STATE_TRANSMITTING) {
      onTxOperationBegin(radio->getTransmissionInProgress());
    }
    txCurrentState = static_cast<IRadio::TransmissionState>(value);
  }
  else if (signalID == IRadio::receptionStateChangedSignal) {
    if (rxCurrentState == IRadio::ReceptionState::RECEPTION_STATE_IDLE &&
        value == IRadio::ReceptionState::RECEPTION_STATE_RECEIVING) {
      onRxOperationBegin(radio->getReceptionInProgress());
    }
    rxCurrentState = static_cast<IRadio::ReceptionState>(value);
  }
  else {
    throw cRuntimeError{"Received unexpected signal \"%s\"", getSignalName(signalID)};
  }
}

}  // namespace smile
