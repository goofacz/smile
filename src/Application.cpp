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

Application::Application() :
    processPendingTxFramesMessage{std::make_unique<omnetpp::cMessage>("processPendingTxFrames")}
{}

Application::~Application()
{
  cancelEvent(processPendingTxFramesMessage.get());
}

void Application::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    measurementsLogger = inet::getModuleFromPar<MeasurementsLogger>(par("measurementsLoggerModule"), this);
    clock = check_and_cast<IClock*>(getModuleByPath("^.clock"));
  }
}

int Application::numInitStages() const
{
  return inet::INITSTAGE_APPLICATION_LAYER + 1;
}

void Application::handleMessage(omnetpp::cMessage* message)
{
  if (message->isSelfMessage()) {
    dispatchSelfMessage(message);
  }
  else {
    dispatchMessage(message);
  }
}

void Application::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID,
                                const omnetpp::SimTime& value, omnetpp::cObject* details)
{
  if (signalID == IClock::windowUpdateSignal) {
    handleWindowUpdateSignal(value);
  }
}

void Application::handleTxStateChanged(inet::physicallayer::IRadio::TransmissionState state)
{
  EV_DETAIL << "Application::handleTxStateChanged) should be overridden!" << endl;
}

void Application::handleRxStateChanged(inet::physicallayer::IRadio::ReceptionState state)
{
  EV_DETAIL << "Application::handleRxStateChanged) should be overridden!" << endl;
}

void Application::handleMessage(std::unique_ptr<omnetpp::cMessage>)
{
  EV_WARN << "Application::handleMessage(std::unique_ptr<omnetpp::cMessage>) should be overridden!" << std::endl;
}

void Application::handleReceivedFrame(std::unique_ptr<inet::MACFrameBase> frame,
                                      const omnetpp::SimTime& receptionTimestamp)
{
  EV_WARN << "Application::handleReceivedFrame() should be overridden!" << endl;
}

void Application::handleTransmittedFrame(const std::unique_ptr<inet::MACFrameBase>& frame,
                                         const omnetpp::SimTime& transmissionTimestamp)
{
  EV_WARN << "Application::handleReceivedFrame() should be overridden!" << endl;
}

void Application::scheduleFrameTransmission(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& delay)
{
  const auto txClockTimestamp = clock->getClockTimestamp() + delay;
  const auto txSimulationTimestamp = clock->convertToSimulationTimestamp(txClockTimestamp);
  if (txSimulationTimestamp) {
    transmitFrame(std::move(frame), *txSimulationTimestamp);
  }
  else {
    storePendingTxFrame(std::move(frame), txClockTimestamp);
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

void Application::storePendingTxFrame(std::unique_ptr<inet::MACFrameBase> frame,
                                      const omnetpp::SimTime& txClockTimestamp)
{
  if (pendingTxFrames.empty()) {
    auto clockModule = check_and_cast<omnetpp::cComponent*>(clock);
    clockModule->subscribe(IClock::windowUpdateSignal, this);
  }

  const auto& destinationAddress = frame->getDestAddr();
  EV_DETAIL << "Scheduling MAC frame ID " << frame->getId() << " addressed to " << destinationAddress.str()
            << " to be transmitted later" << endl;

  auto predicate = [](const auto& timestamp, const auto& pendingFrame) { return timestamp < pendingFrame.second; };
  auto nextFrame = std::upper_bound(pendingTxFrames.begin(), pendingTxFrames.end(), txClockTimestamp, predicate);
  pendingTxFrames.emplace(nextFrame, std::move(frame), txClockTimestamp);
}

void Application::handleWindowUpdateSignal(const omnetpp::SimTime& clockWindowEndTimestamp)
{
  try {
    const auto& pendingFrame = pendingTxFrames.at(0);
    if (pendingFrame.second <= clockWindowEndTimestamp) {
      scheduleAt(0, processPendingTxFramesMessage.get());
    }
  }
  catch (const std::out_of_range&) {
    throw omnetpp::cRuntimeError("Application got IClock::windowUpdateSignal but has no pending TX frames.");
  }
}

void Application::handleProcessPendingTxFramesMessage()
{
  for (auto pendingFrame{pendingTxFrames.begin()}; pendingFrame != pendingTxFrames.end(); ++pendingFrame) {
    auto& frame = pendingFrame->first;
    const auto& txClockTimestamp = pendingFrame->second;
    const auto txSimulationTimestamp = clock->convertToSimulationTimestamp(txClockTimestamp);
    if (!txSimulationTimestamp) {
      break;
    }

    transmitFrame(std::move(frame), *txSimulationTimestamp);
    pendingFrame = pendingTxFrames.erase(pendingFrame);
  }

  if (pendingTxFrames.empty()) {
    auto clockModule = check_and_cast<omnetpp::cComponent*>(clock);
    clockModule->unsubscribe(IClock::windowUpdateSignal, this);
  }
}

void Application::dispatchSelfMessage(omnetpp::cMessage* message)
{
  if (message == processPendingTxFramesMessage.get()) {
    handleProcessPendingTxFramesMessage();
    return;
  }

  handleMessage(std::unique_ptr<omnetpp::cMessage>{message});
}

void Application::dispatchMessage(omnetpp::cMessage* message)
{
  auto frame = dynamic_cast<inet::MACFrameBase*>(message);
  if (frame) {
    const auto clockTimestamp = clock->getClockTimestamp();
    handleReceivedFrame(std::unique_ptr<inet::MACFrameBase>{frame}, clockTimestamp);
    return;
  }

  handleMessage(std::unique_ptr<omnetpp::cMessage>{message});
}

void Application::transmitFrame(std::unique_ptr<inet::MACFrameBase> frame,
                                const omnetpp::SimTime& txSimulationTimestamp)
{
  const auto& destinationAddress = frame->getDestAddr();
  EV_DETAIL << "Passing MAC frame ID " << frame->getId() << " addressed to " << destinationAddress.str()
            << " down to NIC" << endl;

  const auto delay = txSimulationTimestamp - simTime();
  sendDelayed(frame.release(), delay, "out");
}

}  // namespace smile
