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

#include "ApplicationBase.h"
#include <inet/common/ModuleAccess.h>
#include <cassert>

using namespace inet;

namespace smile {

Define_Module(ApplicationBase);

void ApplicationBase::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);
  if (stage == INITSTAGE_LOCAL) {
    try {
      const auto wirelessInterface = getModuleFromPar<cSimpleModule>(par("wirelessInterfaceModule"), this, true);
      radio = dynamic_cast<physicallayer::IRadio*>(wirelessInterface->getModuleByPath(".radio"));
      if (!radio) {
        throw cRuntimeError("Module on path '%s' was not found");
      }
    }
    catch (const std::bad_cast&) {
      throw cRuntimeError("Module on path '%s' failed to cast to physicallayer::IRadio");
    }

    logger = getModuleFromPar<Logger>(par("loggerModule"), this, true);
    mobility = getModuleFromPar<IMobility>(par("mobilityModule"), this, true);
  }
}

inet::physicallayer::IRadio& ApplicationBase::getRadio()
{
  assert(radio);
  return *radio;
}

Logger& ApplicationBase::getLlogger()
{
  assert(logger);
  return *logger;
}

IClock& ApplicationBase::getClock()
{
  assert(clock);
  return *clock;
}

inet::IMobility& ApplicationBase::getMobility()
{
  assert(mobility);
  return *mobility;
}

int ApplicationBase::numInitStages() const
{
  return INITSTAGE_APPLICATION_LAYER + 1;
}

}  // namespace smile
