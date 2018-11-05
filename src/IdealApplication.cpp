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

#include "IdealApplication.h"
#include <inet/common/INETDefs.h>
#include <inet/common/ModuleAccess.h>
#include <inet/linklayer/common/Ieee802Ctrl.h>
#include <algorithm>
#include "utilities.h"

using namespace inet;
using namespace omnetpp;

namespace smile {

Define_Module(IdealApplication);

void IdealApplication::initialize(int stage)
{
  Application::initialize(stage);
  if (stage == INITSTAGE_LOCAL) {
    auto nicDriverModule = check_and_cast<cModule*>(&getNicDriver());
    nicDriverModule->subscribe(IRangingNicDriver::txCompletedSignalId, this);
    nicDriverModule->subscribe(IRangingNicDriver::rxCompletedSignalId, this);
  }

  if (stage == INITSTAGE_LINK_LAYER_2) {
    const auto& nicDriver = getNicDriver();
    macAddress = nicDriver.getMacAddress();
  }
}

void IdealApplication::handleTxCompletionSignal(const IdealTxCompletion&)
{
  EV_WARN_C("IdealApplication") << "Dummy handler handleTxCompletionSignal() was called" << endl;
}

void IdealApplication::handleRxCompletionSignal(const IdealRxCompletion&)
{
  EV_WARN_C("IdealApplication") << "Dummy handler handleRxCompletionSignal() was called" << endl;
}

const MacAddress& IdealApplication::getMacAddress() const
{
  return macAddress;
}

void IdealApplication::initializeFrame(AckingMacHeader& frame, const MacAddress& destinationAddress,
                                       const MacAddress& sourceAddress)
{
  // FIXME auto controlInformation = std::make_unique<Ieee802Ctrl>();
  // controlInformation->setSourceAddress(sourceAddress);
  // controlInformation->setDest(destinationAddress);

  frame.setSrc(sourceAddress);
  frame.setDest(destinationAddress);
  // FIMXE frame.setControlInfo(controlInformation.release());
}

void IdealApplication::receiveSignal(cComponent* source, simsignal_t signalID, cObject* value,
                                     cObject* details)
{
  if (signalID == IRangingNicDriver::txCompletedSignalId) {
    auto completion = check_and_cast<const IdealTxCompletion*>(value);
    handleTxCompletionSignal(*completion);
  }
  else if (signalID == IRangingNicDriver::rxCompletedSignalId) {
    auto completion = check_and_cast<const IdealRxCompletion*>(value);
    handleRxCompletionSignal(*completion);
  }
  else {
    throw cRuntimeError{"Received unexpected signal \"%s\"", getSignalName(signalID)};
  }
}

}  // namespace smile
