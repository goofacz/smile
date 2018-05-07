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

#pragma once

#include <inet/common/INETDefs.h>
#include <omnetpp.h>
#include <algorithm>
#include <list>
#include <memory>
#include <string>
#include <type_traits>
#include "IClock.h"

namespace smile {

namespace clock_decorator_details {

struct Message final
{
  Message() = default;
  Message(std::unique_ptr<cMessage> newMessage, const SimTime& newClockTimestamp, cGate* newGate);

  Message(const Message& source) = delete;
  Message(Message&& source) = default;
  ~Message() = default;

  Message& operator=(const Message& source) = delete;
  Message& operator=(Message&& source) = default;

  std::unique_ptr<cMessage> message;
  SimTime clockTimestamp;
  cGate* gate{nullptr};  // Self messages has nullptr gate
};

};  // namespace clock_decorator_details

template <class BaseModule>
class ClockDecorator : public BaseModule, public cListener
{
  static_assert(std::is_base_of<cModule, BaseModule>::value, "ClockDecorator has t derive from cModule");

 private:
  using ScheduledMessagesList = std::list<clock_decorator_details::Message>;

 public:
  ClockDecorator() = default;
  ClockDecorator(const ClockDecorator& source) = delete;
  ClockDecorator(ClockDecorator&& source) = delete;
  ~ClockDecorator();

  ClockDecorator& operator=(const ClockDecorator& source) = delete;
  ClockDecorator& operator=(ClockDecorator&& source) = delete;

  void scheduleAt(simtime_t clockTimestamp, cMessage* message) override final;

  void sendDelayed(cMessage* message, simtime_t delay, int gateID) override final;

  void sendDelayed(cMessage* message, simtime_t delay, const char* gateName, int gateIndex = -1) override final;

  void sendDelayed(cMessage* message, simtime_t delay, cGate* outputGate) override final;

  SimTime clockTime() const;

 protected:
  void initialize(int stage) override;

  void handleMessage(cMessage* message) override final;

  void receiveSignal(cComponent* source, simsignal_t signalID, const SimTime& value, cObject* details) override;

  virtual void handleSelfMessage(cMessage* message);

  virtual void handleIncommingMessage(cMessage* message);

 private:
  void scheduleMessage(std::unique_ptr<cMessage> message, const SimTime& clockTimestamp, cGate* gate);

  void handleSendScheduledMessagesSelfMessage();

  IClock* clock{nullptr};
  cModule* clockModule{nullptr};
  ScheduledMessagesList scheduledMessages;
  std::unique_ptr<cMessage> sendScheduledMessagesSelfMessage;
};

template <typename BaseModule>
ClockDecorator<BaseModule>::~ClockDecorator()
{
  for (auto& element : scheduledMessages) {
    // Self massages are managed by objects that created them, don't delete them here
    if (!element.gate) {
      element.message.release();
    }
  }

  BaseModule::cancelEvent(sendScheduledMessagesSelfMessage.get());
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::scheduleAt(simtime_t clockTimestamp, cMessage* message)
{
  EV_DEBUG << "Calling ClockDecorator<BaseModule>::scheduleAt()" << endl;
  const auto simulationTime = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTime) {
    BaseModule::scheduleAt(*simulationTime, message);
  }
  else {
    scheduleMessage(std::unique_ptr<cMessage>{message}, clockTimestamp, nullptr);
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::sendDelayed(cMessage* message, simtime_t delay, int gateID)
{
  const auto clockTimestamp = clockTime() + delay;
  const auto simulationTime = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTime) {
    BaseModule::sendDelayed(message, delay, gateID);
  }
  else {
    auto outputGate = BaseModule::gate(gateID);
    scheduleMessage(std::unique_ptr<cMessage>{message}, clockTimestamp, outputGate);
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::sendDelayed(cMessage* message, simtime_t delay, const char* gateName, int gateIndex)
{
  const auto clockTimestamp = clockTime() + delay;
  const auto simulationTime = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTime) {
    BaseModule::sendDelayed(message, delay, gateName, gateIndex);
  }
  else {
    auto outputGate = BaseModule::gate(gateName, gateIndex);
    scheduleMessage(std::unique_ptr<cMessage>{message}, clockTimestamp, outputGate);
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::sendDelayed(cMessage* message, simtime_t delay, cGate* outputGate)
{
  const auto clockTimestamp = clockTime() + delay;
  const auto simulationTime = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTime) {
    BaseModule::sendDelayed(message, delay, outputGate);
  }
  else {
    scheduleMessage(std::unique_ptr<cMessage>{message}, clockTimestamp, outputGate);
  }
}

template <typename BaseModule>
SimTime ClockDecorator<BaseModule>::clockTime() const
{
  return clock->getClockTimestamp();
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::initialize(int stage)
{
  BaseModule::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    clockModule = BaseModule::getModuleByPath(BaseModule::par("clockModule").stringValue());
    if (!clockModule) {
      throw cRuntimeError{"Failed to find clock module at relative path \"%s\"", clockModule};
    }
    clock = check_and_cast<IClock*>(clockModule);

    sendScheduledMessagesSelfMessage = std::make_unique<cMessage>("sendScheduledMessagesSelfMessage");
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::handleMessage(cMessage* message)
{
  if (message->isSelfMessage()) {
    if (message == sendScheduledMessagesSelfMessage.get()) {
      handleSendScheduledMessagesSelfMessage();
    }
    else {
      handleSelfMessage(message);
    }
  }
  else {
    handleIncommingMessage(message);
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::receiveSignal(cComponent* source, simsignal_t signalID, const SimTime& value,
                                               cObject* details)
{
  EV_DEBUG << "Received signal " << BaseModule::getSignalName(signalID) << endl;

  if (signalID == IClock::windowUpdateSignal) {
    const auto& firstMessage = scheduledMessages.front();
    if (firstMessage.clockTimestamp <= value) {
      BaseModule::scheduleAt(simTime(), sendScheduledMessagesSelfMessage.get());
    }
  }
  else {
    throw cRuntimeError{"Received unexpected signal \"%s\"", BaseModule::getSignalName(signalID)};
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::handleSelfMessage(cMessage* message)
{
  throw cRuntimeError{"Default ClockDecorator::handleSelfMessage() implementation received message"};
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::handleIncommingMessage(cMessage* message)
{
  throw cRuntimeError{"Default ClockDecorator::handleIncommingMessage() implementation received message"};
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::scheduleMessage(std::unique_ptr<cMessage> message, const SimTime& clockTimestamp,
                                                 cGate* gate)
{
  if (scheduledMessages.empty()) {
    clockModule->subscribe(IClock::windowUpdateSignal, this);
    EV_DEBUG << "Subscribe on signal " << BaseModule::getSignalName(IClock::windowUpdateSignal) << endl;
  }

  auto predicate = [](const auto& element, const auto& clockTimestamp) {
    return element.clockTimestamp < clockTimestamp ? true : false;
  };

  EV_DETAIL << "Scheduling message \"" << message.get() << "\" according to local clock" << endl;

  auto element = std::lower_bound(scheduledMessages.begin(), scheduledMessages.end(), clockTimestamp, predicate);
  scheduledMessages.emplace(element, std::move(message), clockTimestamp, gate);
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::handleSendScheduledMessagesSelfMessage()
{
  for (auto element = scheduledMessages.begin(); element != scheduledMessages.end();) {
    const auto simulationTime = clock->convertToSimulationTimestamp(element->clockTimestamp);
    if (!simulationTime) {
      break;
    }

    EV_DETAIL << "Sending scheduled message " << element->message.get() << " according to local clock" << endl;

    if (!element->gate) {
      BaseModule::scheduleAt(*simulationTime, element->message.release());
    }
    else {
      const auto delay = *simulationTime - simTime();
      BaseModule::sendDelayed(element->message.release(), delay, element->gate);
    }

    element = scheduledMessages.erase(element);
  }

  if (scheduledMessages.empty()) {
    EV_DEBUG << "Unsubscribe from signal " << BaseModule::getSignalName(IClock::windowUpdateSignal) << endl;
    clockModule->unsubscribe(IClock::windowUpdateSignal, this);
  }
}

}  // namespace smile
