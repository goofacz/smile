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

#include <cassert>

#include "FakeIdealApplication.h"

namespace smile {
namespace fakes {

Define_Module(FakeIdealApplication);

FakeIdealApplication::~FakeIdealApplication()
{
  if (periodicTxMessage) {
    cancelEvent(periodicTxMessage.get());
  }
}

void FakeIdealApplication::initialize(int stage)
{
  IdealApplication::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    periodicTxMessage = std::make_unique<cMessage>("periodicTxMessage");
    if (par("initiator").boolValue()) {
      const auto delay = SimTime{par("txDelay").longValue(), SIMTIME_MS};
      scheduleAt(clockTime() + delay, periodicTxMessage.get());
    }
  }
}

void FakeIdealApplication::handleSelfMessage(cMessage* message)
{
  const auto destinationAddress = inet::MACAddress{par("remoteMacAddress").stringValue()};
  const auto name = std::string{"Test frame no. "} + std::to_string(completedTxOperations);
  auto frame = createFrame<inet::IdealMacFrame>(destinationAddress, name.c_str());
  frame->setBitLength(10);

  EV_DETAIL_C("FakeIdealApplication") << "Sending frame (" << frame->getClassName() << ")\"" << frame->getFullName()
                                      << "\"" << endl;
  send(frame.release(), "out");
  completedTxOperations++;

  if (completedTxOperations < par("initiatorTxCount").longValue()) {
    const auto delay = SimTime{par("txDelay").longValue(), SIMTIME_MS};
    scheduleAt(clockTime() + delay, periodicTxMessage.get());
  }
}

void FakeIdealApplication::handleIncommingMessage(cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  EV_DETAIL_C("FakeIdealApplication") << "Received frame (" << message->getClassName() << ")\""
                                      << message->getFullName() << "\"" << endl;
}

void FakeIdealApplication::handleTxCompletionSignal(const IdealTxCompletion& completion)
{
  EV_DETAIL_C("FakeIdealApplication") << "Transmission of frame (" << completion.getFrame()->getClassName() << ")\""
                                      << completion.getFrame()->getFullName() << "\" started at "
                                      << completion.getOperationBeginClockTimestamp() << endl;
}

void FakeIdealApplication::handleRxCompletionSignal(const IdealRxCompletion& completion)
{
  EV_DETAIL_C("FakeIdealApplication") << "Reception of frame (" << completion.getFrame()->getClassName() << ")\""
                                      << completion.getFrame()->getFullName() << "\" started at "
                                      << completion.getOperationBeginClockTimestamp() << endl;
}

}  // namespace fakes
}  // namespace smile
