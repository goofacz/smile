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

void FakeIdealApplication::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    periodicTxMessage = std::make_unique<omnetpp::cMessage>("periodicTxMessage");
    if (par("initiator").boolValue()) {
      scheduleAt(simTime() + par("txDelay").longValue(), periodicTxMessage.get());
    }
  }
}

void FakeIdealApplication::handleSelfMessage(cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
}

void FakeIdealApplication::handleIncommingMessage(cMessage* message)
{
  const auto destinationAddress = inet::MACAddress{par("remoteAddress").stringValue()};
  auto frame = createFrame<inet::MACFrameBase>(destinationAddress);
  frame->setBitLength(10);

  send(frame.release(), "idealOut");
  scheduleAt(simTime() + par("txDelay").longValue(), periodicTxMessage.get());
}

}  // namespace fakes
}  // namespace smile
