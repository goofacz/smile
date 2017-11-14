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

namespace smile {

Define_Module(IdealApplication);

void IdealApplication::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);
  if (stage == inet::INITSTAGE_LOCAL) {
    mobility = inet::getModuleFromPar<inet::IMobility>(par("mobilityModule"), this, true);

    nicDriver = inet::getModuleFromPar<IRangingNicDriver>(par("nicDriverModule"), this, true);
    auto nicDriverModule = check_and_cast<cModule*>(nicDriver);
    nicDriverModule->subscribe(IRangingNicDriver::txCompletedSignalId, this);
    nicDriverModule->subscribe(IRangingNicDriver::rxCompletedSignalId, this);

    measurementsLogger = inet::getModuleFromPar<MeasurementsLogger>(par("measurementsLoggerModule"), this, true);
  }
}

int IdealApplication::numInitStages() const
{
  return inet::INITSTAGE_APPLICATION_LAYER + 1;
}

void IdealApplication::handleTxCompletionSignal(const IdealTxCompletion&)
{
  EV_WARN_C("IdealApplication") << "Dummy handler handleTxCompletionSignal() was called" << endl;
}

void IdealApplication::handleRxCompletionSignal(const IdealRxCompletion&)
{
  EV_WARN_C("IdealApplication") << "Dummy handler handleRxCompletionSignal() was called" << endl;
}

inet::Coord IdealApplication::getCurrentTruePosition() const
{
  return mobility->getCurrentPosition();
}

void IdealApplication::initializeFrame(inet::IdealMacFrame& frame, const inet::MACAddress& destinationAddress,
                                       const inet::MACAddress& sourceAddress)
{
  auto controlInformation = std::make_unique<inet::Ieee802Ctrl>();
  controlInformation->setSourceAddress(sourceAddress);
  controlInformation->setDest(destinationAddress);

  frame.setSrc(sourceAddress);
  frame.setDest(destinationAddress);
  frame.setControlInfo(controlInformation.release());
}

void IdealApplication::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, cObject* value,
                                     omnetpp::cObject* details)
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
