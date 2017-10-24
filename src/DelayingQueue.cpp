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

#include "DelayingQueue.h"

namespace smile {

Define_Module(DelayingQueue);

void DelayingQueue::initialize(int stage)
{
  cSimpleModule::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    inGate = gate("in");
    outGate = gate("out");

    auto clockModule = getModuleByPath(par("clockModuleRealitivePath").stringValue());
    if (!clockModule) {
      throw omnetpp::cRuntimeError{"Failed to find clock module at relative path \"%s\"",
                                   par("clockModuleRealitivePath").stringValue()};
    }
    clock = check_and_cast<IClock*>(clockModule);
  }
}

void DelayingQueue::handleMessage(omnetpp::cMessage* message)
{
  if (message->isSelfMessage()) {
    handleSelfMessage(message);
  }
  else {
    std::unique_ptr<omnetpp::cMessage> messageHolder{message};
    const auto arrivalgate = messageHolder->getArrivalGate();
    if (arrivalgate == inGate) {
      handleInMessage(std::move(messageHolder));
    }
    else {
      throw omnetpp::cRuntimeError{"Received unexpected message \"%s\" on unknown gate \"%s\"",
                                   messageHolder->getFullName(), arrivalgate->getFullName()};
    }
  }
}

void DelayingQueue::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, const omnetpp::SimTime&,
                                  omnetpp::cObject* details)
{
  // TODO
}

void DelayingQueue::requestPacket()
{
  // Dummy function
}

int DelayingQueue::getNumPendingRequests()
{
  return 0;
}

bool DelayingQueue::isEmpty()
{
  return true;
}

void DelayingQueue::clear()
{
  // Dummy function
}

omnetpp::cMessage* DelayingQueue::pop()
{
  cancelEvent(sendOutSelfMessage.get());
  return scheduledMessage.release();
}

void DelayingQueue::addListener(inet::IPassiveQueueListener* listener)
{
  // TODO
}

void DelayingQueue::removeListener(inet::IPassiveQueueListener* listener)
{
  // TODO
}

void DelayingQueue::handleInMessage(std::unique_ptr<omnetpp::cMessage> message) {}

void DelayingQueue::handleSelfMessage(omnetpp::cMessage* message)
{
  if (message == sendOutSelfMessage.get()) {
    // TODO
  }
  else {
    EV_ERROR << "Received unknown self message " << message->getFullName() << endl;
  }
}

}  // namespace smile
