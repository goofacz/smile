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
    measurementsLogger =
        inet::getModuleFromPar<MeasurementsLogger>(par("measurementsLogger"), this, true);
  }
}

void Application::handleMessage(omnetpp::cMessage* message)
{
  handleMessage(std::unique_ptr<omnetpp::cMessage>{message});
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

}  // namespace smile
