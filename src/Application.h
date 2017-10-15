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
#include "MeasurementsLogger.h"
#include "RadioNode.h"
#include "inet/linklayer/base/MACFrameBase_m.h"
#include "omnetpp.h"

namespace smile {

class Application : public omnetpp::cSimpleModule, public omnetpp::cListener, public IApplication
{
 private:
  using PendingTxFrame = std::pair<std::unique_ptr<inet::MACFrameBase>, omnetpp::SimTime>;

 public:
  Application();
  Application(const Application& source) = delete;
  Application(Application&& source) = delete;
  ~Application() override = default;

  Application& operator=(const Application& source) = delete;
  Application& operator=(Application&& source) = delete;

  template <typename Frame, typename... FrameArguments>
  std::unique_ptr<Frame> createMACFrame(const inet::MACAddress& destinationAddress, FrameArguments&&... frameArguments);

 protected:
  void initialize(int stage) override;

  int numInitStages() const final;

  void handleMessage(omnetpp::cMessage* message) final;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, const omnetpp::SimTime& value,
                     omnetpp::cObject* details) override;

  void handleTxStateChanged(inet::physicallayer::IRadio::TransmissionState state) override;

  void handleRxStateChanged(inet::physicallayer::IRadio::ReceptionState state) override;

  virtual void handleMessage(std::unique_ptr<omnetpp::cMessage> message);

  virtual void handleReceivedFrame(std::unique_ptr<inet::MACFrameBase> frame,
                                   const omnetpp::SimTime& receptionTimestamp);

  virtual void handleTransmittedFrame(const std::unique_ptr<inet::MACFrameBase>& frame,
                                      const omnetpp::SimTime& transmissionTimestamp);

  template <typename Frame>
  void scheduleFrameTransmission(std::unique_ptr<Frame> frame, const omnetpp::SimTime& delay);

  void scheduleFrameTransmission(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& delay);

 private:
  static void prepareFrame(inet::MACFrameBase& frame, const inet::MACAddress& destinationAddress,
                           const inet::MACAddress& sourceAddress);

  void handleWindowUpdateSignal(const omnetpp::SimTime& clockWindowEndTimestamp);

  void handleProcessPendingTxFramesMessage();

  void dispatchSelfMessage(omnetpp::cMessage* message);

  void dispatchMessage(omnetpp::cMessage* message);

  void storePendingTxFrame(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& txClockTimestamp);

  void transmitFrame(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& txSimulationTimestamp);

  MeasurementsLogger* measurementsLogger{nullptr};
  IClock* clock{nullptr};
  RadioNode* radioNode{nullptr};
  std::vector<PendingTxFrame> pendingTxFrames;
  const std::unique_ptr<omnetpp::cMessage> processPendingTxFramesMessage;
};

template <typename Frame, typename... FrameArguments>
std::unique_ptr<Frame> Application::createMACFrame(const inet::MACAddress& destinationAddress,
                                                   FrameArguments&&... frameArguments)
{
  static_assert(std::is_base_of<inet::MACFrameBase, Frame>::value,
                "Application::createMACFrame requires Frame to derive from inet::MACFrameBase");

  auto frame = std::make_unique<Frame>(frameArguments...);
  const auto& localAddress = radioNode->getMACAddress();
  prepareFrame(*frame, destinationAddress, localAddress);
  return frame;
}

template <typename Frame>
void Application::scheduleFrameTransmission(std::unique_ptr<Frame> frame, const omnetpp::SimTime& delay)
{
  static_assert(std::is_base_of<inet::MACFrameBase, Frame>::value,
                "Application::createMACFrame requires Frame to derive from inet::MACFrameBase");

  std::unique_ptr<inet::MACFrameBase> baseFrame{static_cast<inet::MACFrameBase*>(frame.release())};
  scheduleFrameTransmission(std::move(baseFrame), delay);
}

}  // namespace smile
