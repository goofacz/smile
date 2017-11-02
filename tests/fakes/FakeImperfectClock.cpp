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

#include "FakeImperfectClock.h"

namespace smile {
namespace fakes {

Define_Module(FakeImperfectClock);

FakeImperfectClock::FakeImperfectClock() :
    windowUpdateSelfMessage{std::make_unique<omnetpp::cMessage>("windowUpdateSelfMessage")}
{}

FakeImperfectClock::~FakeImperfectClock()
{
  cancelEvent(windowUpdateSelfMessage.get());
}

void FakeImperfectClock::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_LOCAL) {
    currentWindowEndTimestamp = simTime() + windowDuration;
    scheduleAt(currentWindowEndTimestamp, windowUpdateSelfMessage.get());

    EV_DEBUG << "Initialize FakeImperfectClock:" << endl;
  }
}

void FakeImperfectClock::handleMessage(omnetpp::cMessage* message)
{
  if (!message->isSelfMessage()) {
    throw omnetpp::cRuntimeError{"FakeImperfectClock: received unexpected message"};
  }

  currentWindowEndTimestamp = simTime() + windowDuration;
  scheduleAt(currentWindowEndTimestamp, windowUpdateSelfMessage.get());

  emit(IClock::windowUpdateSignal, currentWindowEndTimestamp);

  EV_DEBUG << "FakeImperfectClock: emit periodic signal " << IClock::windowUpdateSignal << endl;
}

omnetpp::SimTime FakeImperfectClock::getClockTimestamp()
{
  return simTime();
}

FakeImperfectClock::OptionalSimTime FakeImperfectClock::convertToSimulationTimestamp(const omnetpp::SimTime& timestamp)
{
  return timestamp > currentWindowEndTimestamp ? std::experimental::nullopt : OptionalSimTime{timestamp};
}

}  // namespace fakes
}  // namespace smile
