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

#include "PeriodicSenderApplication.h"
#include "inet/linklayer/base/MACFrameBase_m.h"

namespace smile {
namespace mocks {

Define_Module(PeriodicSenderApplication);

PeriodicSenderApplication::PeriodicSenderApplication() :
    periodicTxMessage{std::make_unique<omnetpp::cMessage>("periodicTxMessage")}
{}

PeriodicSenderApplication::~PeriodicSenderApplication()
{
  cancelEvent(periodicTxMessage.get());
}

void PeriodicSenderApplication::initialize(int stage)
{
  Application::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    scheduleAt(simTime() + delay, periodicTxMessage.get());
  }
}

void PeriodicSenderApplication::handleTransmissionCompleted(std::unique_ptr<inet::MACFrameBase>,
                                                            const omnetpp::SimTime&)
{}

void PeriodicSenderApplication::handleMessage(std::unique_ptr<omnetpp::cMessage> message)
{
  if (message.get() != periodicTxMessage.get()) {
    throw omnetpp::cRuntimeError{"Received unexpected message"};
  }

  auto frame = createFrame<inet::MACFrameBase>(destinationAddress);
  frame->setBitLength(10);

  EV_DETAIL << "TEST: Sending MAC frame to " << destinationAddress.str() << endl;

  scheduleFrameTransmission(std::move(frame), 0, false);
  scheduleAt(simTime() + delay, message.release());

  destinationAddress = inet::MACAddress{destinationAddress.getInt() + 1};
}

}  // namespace mocks
}  // namespace smile
