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

#include "ClockTester.h"
#include <inet/common/ModuleAccess.h>
#include "IClock.h"

namespace smile {
namespace testers {

Define_Module(ClockTester);

using DriftRange = std::pair<double, double>;

std::string simtime_to_string(const SimTime& timestamp)
{
  return timestamp.format(SimTime::getScaleExp(), ".", "", true, "", " ");
}

void checkTimestamp(const SimTime& timestamp, const SimTime& reference_timestamp, const DriftRange& drift_range)
{
  const auto left_min_time = reference_timestamp - reference_timestamp * drift_range.second;
  const auto left_max_time = reference_timestamp - reference_timestamp * drift_range.first;
  const auto right_min_time = reference_timestamp + reference_timestamp * drift_range.first;
  const auto right_max_time = reference_timestamp + reference_timestamp * drift_range.second;

  EV_INFO << "CHECK if converted time is in range: \n";
  EV_INFO << "\tReference time: " << simtime_to_string(reference_timestamp) << "\n";
  EV_INFO << "\tConverted time: " << simtime_to_string(timestamp) << "\n";
  EV_INFO << "\tLeft time range:  < " << simtime_to_string(left_min_time) << ", " << simtime_to_string(left_max_time)
          << "> \n";
  EV_INFO << "\tRight time range: < " << simtime_to_string(right_min_time) << ", " << simtime_to_string(right_max_time)
          << "> \n";

  const auto left_condition = (timestamp >= left_min_time) && (timestamp <= left_max_time);
  const auto right_condition = (timestamp >= right_min_time) && (timestamp <= right_max_time);
  if (!left_condition && !right_condition) {
    EV_INFO << "\t!!! FAILURE !!!\n";
    throw cRuntimeError{"Test failed"};
  }

  EV_INFO << "\tSUCCESS\n";
}

void ClockTester::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);

  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    // Access tested clock module
    auto clockModule = getModuleByPath("^.clock");
    if (!clockModule) {
      throw cRuntimeError{"Clock module was not found"};
    }

    clock = check_and_cast<IClock*>(clockModule);

    const auto drift_range = std::make_pair(par("min_drift").doubleValue(), par("max_drift").doubleValue());
    EV_INFO << "Absolute drift range: < " << drift_range.first << ", " << drift_range.second << " >\n";

    // Test time conversion (from hardware clock to simulation clock)
    {
      const auto referenceTime = SimTime(10, SIMTIME_NS);
      checkTimestamp(*(clock->convertToSimulationTimestamp(referenceTime)), referenceTime, drift_range);
    }

    {
      const auto referenceTime = SimTime(10, SIMTIME_US);
      checkTimestamp(*(clock->convertToSimulationTimestamp(referenceTime)), referenceTime, drift_range);
    }

    {
      const auto referenceTime = SimTime(10, SIMTIME_MS);
      checkTimestamp(*(clock->convertToSimulationTimestamp(referenceTime)), referenceTime, drift_range);
    }

    {
      const auto referenceTime = SimTime(10, SIMTIME_S);
      checkTimestamp(*(clock->convertToSimulationTimestamp(referenceTime)), referenceTime, drift_range);
    }

    // Send messages

    // TODO
  }
}

int ClockTester::numInitStages() const
{
  return inet::INITSTAGE_APPLICATION_LAYER + 1;
}

}  // namespace testers
}  // namespace smile
