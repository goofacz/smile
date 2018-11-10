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
  Message(std::unique_ptr<omnetpp::cMessage> newMessage, const omnetpp::SimTime& newClockTimestamp,
          omnetpp::cGate* newGate);

  Message(const Message& source) = delete;
  Message(Message&& source) = default;
  ~Message() = default;

  Message& operator=(const Message& source) = delete;
  Message& operator=(Message&& source) = default;

  std::unique_ptr<omnetpp::cMessage> message;
  omnetpp::SimTime clockTimestamp;
  omnetpp::cGate* gate{nullptr};  // Self messages has nullptr gate
};

};  // namespace clock_decorator_details

template <class BaseModule>
class ClockDecorator : public BaseModule, public omnetpp::cListener
{
  static_assert(std::is_base_of<omnetpp::cModule, BaseModule>::value, "ClockDecorator has t derive from cModule");

 private:
  using ScheduledMessagesList = std::list<clock_decorator_details::Message>;

 public:
  ClockDecorator() = default;
  ClockDecorator(const ClockDecorator& source) = delete;
  ClockDecorator(ClockDecorator&& source) = delete;
  ~ClockDecorator();

  ClockDecorator& operator=(const ClockDecorator& source) = delete;
  ClockDecorator& operator=(ClockDecorator&& source) = delete;

  void scheduleAt(omnetpp::simtime_t clockTimestamp, omnetpp::cMessage* message) override final;

  void sendDelayed(omnetpp::cMessage* message, omnetpp::simtime_t delay, int gateID) override final;

  void sendDelayed(omnetpp::cMessage* message, omnetpp::simtime_t delay, const char* gateName,
                   int gateIndex = -1) override final;

  void sendDelayed(omnetpp::cMessage* message, omnetpp::simtime_t delay, omnetpp::cGate* outputGate) override final;

  omnetpp::SimTime clockTime() const;

 protected:
  void initialize(int stage) override;

  void handleMessage(omnetpp::cMessage* message) override final;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, const omnetpp::SimTime& value,
                     omnetpp::cObject* details) override;

  virtual void handleSelfMessage(omnetpp::cMessage* message);

  virtual void handleIncommingMessage(omnetpp::cMessage* message);

 private:
  void scheduleMessage(std::unique_ptr<omnetpp::cMessage> message, const omnetpp::SimTime& clockTimestamp,
                       omnetpp::cGate* gate);

  void handleSendScheduledMessagesSelfMessage();

  IClock* clock{nullptr};
  omnetpp::cModule* clockModule{nullptr};
  ScheduledMessagesList scheduledMessages;
  std::unique_ptr<omnetpp::cMessage> sendScheduledMessagesSelfMessage;
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
void ClockDecorator<BaseModule>::scheduleAt(omnetpp::simtime_t clockTimestamp, omnetpp::cMessage* message)
{
  EV_DEBUG << "Calling ClockDecorator<BaseModule>::scheduleAt()" << omnetpp::endl;

  const auto simulationTime = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTime) {
    BaseModule::scheduleAt(*simulationTime, message);
  }
  else {
    scheduleMessage(std::unique_ptr<omnetpp::cMessage>{message}, clockTimestamp, nullptr);
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::sendDelayed(omnetpp::cMessage* message, omnetpp::simtime_t delay, int gateID)
{
  const auto clockTimestamp = clockTime() + delay;
  const auto simulationTime = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTime) {
    BaseModule::sendDelayed(message, delay, gateID);
  }
  else {
    auto outputGate = BaseModule::gate(gateID);
    scheduleMessage(std::unique_ptr<omnetpp::cMessage>{message}, clockTimestamp, outputGate);
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::sendDelayed(omnetpp::cMessage* message, omnetpp::simtime_t delay, const char* gateName,
                                             int gateIndex)
{
  const auto clockTimestamp = clockTime() + delay;
  const auto simulationTime = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTime) {
    BaseModule::sendDelayed(message, delay, gateName, gateIndex);
  }
  else {
    auto outputGate = BaseModule::gate(gateName, gateIndex);
    scheduleMessage(std::unique_ptr<omnetpp::cMessage>{message}, clockTimestamp, outputGate);
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::sendDelayed(omnetpp::cMessage* message, omnetpp::simtime_t delay,
                                             omnetpp::cGate* outputGate)
{
  const auto clockTimestamp = clockTime() + delay;
  const auto simulationTime = clock->convertToSimulationTimestamp(clockTimestamp);
  if (simulationTime) {
    BaseModule::sendDelayed(message, delay, outputGate);
  }
  else {
    scheduleMessage(std::unique_ptr<omnetpp::cMessage>{message}, clockTimestamp, outputGate);
  }
}

template <typename BaseModule>
omnetpp::SimTime ClockDecorator<BaseModule>::clockTime() const
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
      throw omnetpp::cRuntimeError{"Failed to find clock module at relative path \"%s\"", clockModule};
    }
    clock = omnetpp::check_and_cast<IClock*>(clockModule);

    sendScheduledMessagesSelfMessage = std::make_unique<omnetpp::cMessage>("sendScheduledMessagesSelfMessage");
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::handleMessage(omnetpp::cMessage* message)
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
void ClockDecorator<BaseModule>::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID,
                                               const omnetpp::SimTime& value, omnetpp::cObject* details)
{
  EV_DEBUG << "Received signal " << BaseModule::getSignalName(signalID) << omnetpp::endl;

  if (signalID == IClock::windowUpdateSignal) {
    const auto& firstMessage = scheduledMessages.front();
    if (firstMessage.clockTimestamp <= value) {
      BaseModule::scheduleAt(omnetpp::simTime(), sendScheduledMessagesSelfMessage.get());
    }
  }
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::handleSelfMessage(omnetpp::cMessage* message)
{
  throw omnetpp::cRuntimeError{"Default ClockDecorator::handleSelfMessage() implementation received message"};
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::handleIncommingMessage(omnetpp::cMessage* message)
{
  throw omnetpp::cRuntimeError{"Default ClockDecorator::handleIncommingMessage() implementation received message"};
}

template <typename BaseModule>
void ClockDecorator<BaseModule>::scheduleMessage(std::unique_ptr<omnetpp::cMessage> message,
                                                 const omnetpp::SimTime& clockTimestamp, omnetpp::cGate* gate)
{
  if (scheduledMessages.empty()) {
    clockModule->subscribe(IClock::windowUpdateSignal, this);
    EV_DEBUG << "Subscribe on signal " << BaseModule::getSignalName(IClock::windowUpdateSignal) << omnetpp::endl;
  }

  auto predicate = [](const auto& element, const auto& clockTimestamp) {
    return element.clockTimestamp < clockTimestamp ? true : false;
  };

  EV_DETAIL << "Scheduling message \"" << message.get() << "\" according to local clock" << omnetpp::endl;

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

    EV_DETAIL << "Sending scheduled message " << element->message.get() << " according to local clock" << omnetpp::endl;

    if (!element->gate) {
      BaseModule::scheduleAt(*simulationTime, element->message.release());
    }
    else {
      const auto delay = *simulationTime - omnetpp::simTime();
      BaseModule::sendDelayed(element->message.release(), delay, element->gate);
    }

    element = scheduledMessages.erase(element);
  }

  if (scheduledMessages.empty()) {
    EV_DEBUG << "Unsubscribe from signal " << BaseModule::getSignalName(IClock::windowUpdateSignal) << omnetpp::endl;
    clockModule->unsubscribe(IClock::windowUpdateSignal, this);
  }
}

}  // namespace smile
