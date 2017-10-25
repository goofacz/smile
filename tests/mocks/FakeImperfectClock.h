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

#pragma once

#include <memory>
#include "IClock.h"
#include "omnetpp.h"

namespace smile {
namespace mocks {

class FakeImperfectClock : public omnetpp::cSimpleModule, public IClock
{
 public:
  FakeImperfectClock();
  FakeImperfectClock(const FakeImperfectClock& source) = delete;
  FakeImperfectClock(FakeImperfectClock&& source) = delete;
  ~FakeImperfectClock();

  FakeImperfectClock& operator=(const FakeImperfectClock& source) = delete;
  FakeImperfectClock& operator=(FakeImperfectClock&& source) = delete;

  omnetpp::SimTime getClockTimestamp() override;
  OptionalSimTime convertToSimulationTimestamp(const omnetpp::SimTime& timestamp) override;

 protected:
  void initialize(int stage) override;

  void handleMessage(omnetpp::cMessage* message) override;

 private:
  const omnetpp::SimTime windowDuration{500, omnetpp::SIMTIME_MS};
  omnetpp::SimTime currentWindowEncTimestamp;
  std::unique_ptr<omnetpp::cMessage> windowUpdateSelfMessage;
};

}  // namespace mocks
}  // namespace smile
