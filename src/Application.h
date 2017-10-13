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
#include <queue>
#include "IApplication.h"
#include "IClock.h"
#include "MeasurementsLogger.h"
#include "inet/linklayer/base/MACFrameBase_m.h"
#include "omnetpp.h"

namespace smile {

class Application : public omnetpp::cSimpleModule, public IApplication
{
 public:
  struct PendingTxFrame
  {
    PendingTxFrame(std::unique_ptr<inet::MACFrameBase> newFrame, const omnetpp::SimTime& newClockTimestamp);
    PendingTxFrame(const PendingTxFrame& source) = delete;
    PendingTxFrame(PendingTxFrame&& source) = default;
    ~PendingTxFrame() = default;

    PendingTxFrame& operator=(const PendingTxFrame& source) = delete;
    PendingTxFrame& operator=(PendingTxFrame&& source) = default;

    bool operator<(const PendingTxFrame& instance) const noexcept;

    std::unique_ptr<inet::MACFrameBase> frame;
    omnetpp::SimTime clockTimestamp;
  };

 public:
  Application() = default;
  Application(const Application& source) = delete;
  Application(Application&& source) = delete;
  ~Application() override = default;

  Application& operator=(const Application& source) = delete;
  Application& operator=(Application&& source) = delete;

 protected:
  void initialize(int stage) override;

  virtual void handleMessage(std::unique_ptr<omnetpp::cMessage> msg);

  virtual void handleReceivedFrame(std::unique_ptr<inet::MACFrameBase> frame,
                                   const omnetpp::SimTime& receptionTimestamp);

  virtual void handleTransmittedFrame(const std::unique_ptr<inet::MACFrameBase>& frame,
                                      const omnetpp::SimTime& transmissionTimestamp);

  void scheduleFrameTransmission(std::unique_ptr<inet::MACFrameBase> frame, const omnetpp::SimTime& delay);

  int numInitStages() const final;

  void handleMessage(omnetpp::cMessage* msg) final;

 private:
  MeasurementsLogger* measurementsLogger{nullptr};
  IClock* clock{nullptr};
  std::priority_queue<PendingTxFrame> pendingTxFrames;
};

}  // namespace smile
