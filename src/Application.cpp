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

#include "Application.h"
#include <inet/common/ModuleAccess.h>
#include <cassert>

using namespace inet;

namespace smile {

Define_Module(Application);

void Application::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);
  if (stage == INITSTAGE_LOCAL) {
    mobility = getModuleFromPar<IMobility>(par("mobilityModule"), this, true);
    nicDriver = getModuleFromPar<IRangingNicDriver>(par("nicDriverModule"), this, true);
  }
}

int Application::numInitStages() const
{
  return INITSTAGE_APPLICATION_LAYER + 1;
}

Coord Application::getCurrentTruePosition() const
{
  return mobility->getCurrentPosition();
}

IRangingNicDriver& Application::getNicDriver()
{
  assert(nicDriver);
  return *nicDriver;
}

}  // namespace smile
