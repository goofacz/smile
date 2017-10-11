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

#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"

#include "Application.h"

namespace smile {

Define_Module(Application);

void Application::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    auto& measurementsLoggerParaeter = par("measurementsLoggerModule");
    measurementsLogger = inet::getModuleFromPar<MeasurementsLogger>(measurementsLoggerParaeter, this, true);

    clock = check_and_cast<IClock*>(getModuleByPath("^.clock"));
  }
}

void Application::handleMessage(omnetpp::cMessage* message)
{
  auto frame = dynamic_cast<inet::MACFrameBase*>(message);
  if (frame) {
    const auto clockTimestamp = clock->getClockTimestamp();
    handleReceivedFrame(std::unique_ptr<inet::MACFrameBase>{frame}, clockTimestamp);
  } else {
    handleMessage(std::unique_ptr<omnetpp::cMessage>{message});
  }
}

int Application::numInitStages() const
{
  return inet::INITSTAGE_APPLICATION_LAYER + 1;
}

void Application::handleMessage(std::unique_ptr<omnetpp::cMessage> msg)
{
  EV_WARN << "Application::handleMessage(std::unique_ptr<omnetpp::cMessage>) should be overridden!"
          << std::endl;
}

void Application::handleReceivedFrame(std::unique_ptr<inet::MACFrameBase> frame,
                                      const omnetpp::SimTime& receptionTimestamp)
{
  EV_DETAIL << "Application::handleReceivedFrame) should be overridden!" << endl;
}

void Application::handleTransmittedFrame(const std::unique_ptr<inet::MACFrameBase>& frame,
                                         const omnetpp::SimTime& transmissionTimestamp)
{
  EV_DETAIL << "Application::handleReceivedFrame) should be overridden!" << endl;
}

void Application::scheduleFrameTransmission(std::unique_ptr<inet::MACFrameBase> frame,
                                            const omnetpp::SimTime& delay)
{
  const auto currentTimestamp = simTime();
  const auto futureTimestamp = clock->getSimulationTimestamp(delay);
  const auto correctedDelay = futureTimestamp - currentTimestamp;
  sendDelayed(frame.release(), correctedDelay, "out");
}

}  // namespace smile
