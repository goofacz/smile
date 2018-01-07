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
#include "SimpleClock.h"

namespace smile {

Define_Module(SimpleClock);

omnetpp::SimTime SimpleClock::getClockTimestamp()
{
  updateError();
  return simTime() + currentError.accumulatedError;
}

SimpleClock::OptionalSimTime SimpleClock::convertToSimulationTimestamp(const SimTime& timestamp)
{
  const auto currentClockTimestamp = getClockTimestamp();
  if (timestamp < currentClockTimestamp) {
    throw cRuntimeError{"Cannot convert past clock timestamp to simulation timestamp"};
  }

  const std::array<omnetpp::SimTimeUnit, 4> units = {omnetpp::SIMTIME_MS, omnetpp::SIMTIME_US, omnetpp::SIMTIME_NS,
                                                     omnetpp::SIMTIME_PS};

  SimTime simulationTimestamp;
  for (const auto unit : units) {
    auto nextSimulationTimestamp = simulationTimestamp;
    while (nextSimulationTimestamp + computeError(nextSimulationTimestamp - simulationTimestamp) < timestamp) {
      simulationTimestamp = nextSimulationTimestamp;
      nextSimulationTimestamp += SimTime(1, unit);
    }
  }

  return simulationTimestamp;
}

void SimpleClock::initialize(int stage)
{
  Clock::initialize(stage);
  if (stage == inet::INITSTAGE_LOCAL) {
    d = par("d").doubleValue();
    epsilon = par("epsilon").doubleValue();
  }
}

SimTime SimpleClock::computeError(const SimTime& deltaTimestamp)
{
  return 0.5 * d * std::pow(deltaTimestamp.dbl(), 2);
}

void SimpleClock::updateError()
{
  const auto deltaTimestamp = simTime() - currentError.simulationTimestamp;
  if (!deltaTimestamp.isZero()) {
    currentError.accumulatedError += computeError(deltaTimestamp);
  }
}

}  // namespace smile
