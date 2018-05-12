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

#include <array>
#include <cmath>

#include <inet/common/INETDefs.h>
#include "LiuYangClock.h"

namespace smile {

Define_Module(LiuYangClock);

omnetpp::SimTime LiuYangClock::getClockTimestamp()
{
  const auto timestamp = simTime();
  return timestamp + (timestamp * computeErrorCoefficient(timestamp));
}

LiuYangClock::OptionalSimTime LiuYangClock::convertToSimulationTimestamp(const SimTime& timestamp)
{
  return timestamp + (timestamp * computeErrorCoefficient(timestamp));
}

void LiuYangClock::initialize(int stage)
{
  Clock::initialize(stage);
  if (stage == inet::INITSTAGE_LOCAL) {
    d = par("__constant_drift").doubleValue();
  }
}

double LiuYangClock::computeErrorCoefficient(const SimTime& timestamp) const
{
  return 0.5 * d * std::pow(timestamp.dbl(), 2.);
}

}  // namespace smile
