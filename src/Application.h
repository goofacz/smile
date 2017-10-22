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
#include <type_traits>
#include <vector>
#include "IApplication.h"
#include "IClock.h"
#include "IRangingWirelessNic.h"
#include "MeasurementsLogger.h"
#include "inet/linklayer/base/MACFrameBase_m.h"
#include "omnetpp.h"

namespace smile {

class Application : public omnetpp::cSimpleModule, public omnetpp::cListener, public IApplication
{
 private:
  using SelfMessagePair = std::pair<omnetpp::cMessage*, omnetpp::SimTime>;
  using SelfMessageVector = std::vector<SelfMessagePair>;

 public:
  Application() = default;
  Application(const Application& source) = delete;
  Application(Application&& source) = delete;
  ~Application() = default;

  Application& operator=(const Application& source) = delete;
  Application& operator=(Application&& source) = delete;

 protected:
  void initialize(int stage) override;

  void receiveSignal(cComponent* source, simsignal_t signalID, cObject* value, cObject* details) override;

  virtual void handleMessage(std::unique_ptr<omnetpp::cMessage> message);

  virtual void handleTransmissionCompleted(std::unique_ptr<inet::MACFrameBase> frame,
                                           const omnetpp::SimTime& clockTimestamp);

  virtual void handleReceptionCompleted(std::unique_ptr<inet::MACFrameBase> frame,
                                        const omnetpp::SimTime& clockTimestamp);

  template <typename Frame, typename... FrameArguments>
  std::unique_ptr<Frame> createFrame(const inet::MACAddress& destinationAddress, FrameArguments&&... frameArguments);

  template <typename Frame>
  bool scheduleFrameTransmission(std::unique_ptr<Frame> frame, const omnetpp::SimTime& delay,
                                 bool cancelScheduledOperation);

  bool scheduleFrameReception(const omnetpp::SimTime& delay, bool cancelScheduledOperation);

  void scheduleAt(omnetpp::SimTime delay, omnetpp::cMessage* message) override;

 private:
  static void initializeFrame(inet::MACFrameBase& frame, const inet::MACAddress& destinationAddress,
                              const inet::MACAddress& sourceAddress);

  int numInitStages() const final;

  void handleMessage(omnetpp::cMessage* message) final;

  void handleWindowUpdateSignal(const omnetpp::SimTime& clockWindowEndTimestamp);

  void handleSelfMessage(omnetpp::cMessage* message);

  MeasurementsLogger* measurementsLogger{nullptr};
  IClock* clock{nullptr};
  IRangingWirelessNic* nic{nullptr};

  SelfMessageVector pendingSlefMessages;
};

template <typename Frame, typename... FrameArguments>
std::unique_ptr<Frame> Application::createFrame(const inet::MACAddress& destinationAddress,
                                                FrameArguments&&... frameArguments)
{
  constexpr auto isDerived = std::is_base_of<inet::MACFrameBase, Frame>::value;
  constexpr auto isSame = std::is_same<inet::MACFrameBase, Frame>::value;
  static_assert(isDerived || isSame,
                "Application::scheduleFrameTransmission requires Frame to derive from inet::MACFrameBase");

  auto frame = std::make_unique<Frame>(std::forward<FrameArguments>(frameArguments)...);
  const auto& localAddress = nic->getMacAddress();
  initializeFrame(*frame, destinationAddress, localAddress);
  return frame;
}

template <typename Frame>
bool Application::scheduleFrameTransmission(std::unique_ptr<Frame> frame, const omnetpp::SimTime& delay,
                                            bool cancelScheduledOperation)
{
  constexpr auto isDerived = std::is_base_of<inet::MACFrameBase, Frame>::value;
  constexpr auto isSame = std::is_same<inet::MACFrameBase, Frame>::value;
  static_assert(isDerived || isSame,
                "Application::scheduleFrameTransmission requires Frame to derive from inet::MACFrameBase");

  const auto result = nic->configureDelayedTransmission(delay, cancelScheduledOperation);
  if (!result) {
    return false;
  }

  send(frame.release(), "out");
  return true;
}

}  // namespace smile
