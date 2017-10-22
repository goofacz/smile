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

#include <algorithm>
#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"

#include "Application.h"

namespace smile {

Define_Module(Application);

void Application::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    measurementsLogger = inet::getModuleFromPar<MeasurementsLogger>(par("measurementsLoggerModule"), this);
    clock = check_and_cast<IClock*>(getModuleByPath("^.clock"));
    nic = check_and_cast<IRangingWirelessNic*>(getModuleByPath("^.nic"));
  }
}

int Application::numInitStages() const
{
  return inet::INITSTAGE_APPLICATION_LAYER + 1;
}

void Application::handleMessage(omnetpp::cMessage* message)
{
  if (message->isSelfMessage()) {
    handleSelfMessage(message);
  }
  else {
    handleMessage(std::unique_ptr<omnetpp::cMessage>{message});
  }
}

void Application::handleMessage(std::unique_ptr<omnetpp::cMessage>)
{
  EV_WARN << "Application::handleMessage(std::unique_ptr<omnetpp::cMessage>) should be overridden!" << std::endl;
}

bool Application::scheduleFrameReception(const omnetpp::SimTime& delay, bool cancelScheduledOperation)
{
  return nic->configureDelayedReception(delay, cancelScheduledOperation);
}

void Application::scheduleAt(std::unique_ptr<cMessage> message, omnetpp::SimTime delay)
{
  const auto futureClockTimestamp = clock->getClockTimestamp() + delay;
  const auto futureSimulationTimestamp = clock->convertToSimulationTimestamp(futureClockTimestamp);
  if (futureSimulationTimestamp) {
    scheduleAt(*futureSimulationTimestamp, message.release());
  }
  else {
    auto nextMessage =
        std::upper_bound(pendingSlefMessages.begin(), pendingSlefMessages.end(), futureClockTimestamp,
                         [](const omnetpp::SimTime& clockTimestamp, const SelfMessagePair& pendingMessage) {
                           return clockTimestamp < pendingMessage.second;
                         });
    if (pendingSlefMessages.empty()) {
      subscribe(IClock::windowUpdateSignal, this);
    }
    pendingSlefMessages.emplace(nextMessage, std::move(message), futureClockTimestamp);
  }
}

void Application::initializeFrame(inet::MACFrameBase& frame, const inet::MACAddress& destinationAddress,
                                  const inet::MACAddress& sourceAddress)
{
  auto controlInformation = std::make_unique<inet::Ieee802Ctrl>();
  controlInformation->setSourceAddress(sourceAddress);
  controlInformation->setDest(destinationAddress);

  frame.setSrcAddr(sourceAddress);
  frame.setDestAddr(destinationAddress);
  frame.setControlInfo(controlInformation.release());
}

void Application::handleWindowUpdateSignal(const omnetpp::SimTime&)
{
  for (auto message = pendingSlefMessages.begin(); message != pendingSlefMessages.end(); ++message) {
    const auto simulationClockTime = clock->convertToSimulationTimestamp(message->second);
    if (!simulationClockTime) {
      break;
    }
    else {
      scheduleAt(*simulationClockTime, message->first.release());
      message = pendingSlefMessages.erase(message);
    }
  }

  if (pendingSlefMessages.empty()) {
    unsubscribe(IClock::windowUpdateSignal, this);
  }
}

void Application::handleSelfMessage(omnetpp::cMessage* message)
{
  // TODO
}

}  // namespace smile
