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

#include "Application.h"
#include "omnetpp.h"

namespace smile {
namespace mocks {

class PeriodicSenderApplication final : public Application
{
 public:
  PeriodicSenderApplication();
  PeriodicSenderApplication(const PeriodicSenderApplication& source) = delete;
  PeriodicSenderApplication(PeriodicSenderApplication&& source) = delete;
  ~PeriodicSenderApplication();

  PeriodicSenderApplication& operator=(const PeriodicSenderApplication& source) = delete;
  PeriodicSenderApplication& operator=(PeriodicSenderApplication&& source) = delete;

 protected:
  void initialize(int stage) final;

  void handleTransmittedFrame(const std::unique_ptr<inet::MACFrameBase>& frame,
                              const omnetpp::SimTime& transmissionTimestamp) final;

 private:
  void handleMessage(std::unique_ptr<omnetpp::cMessage> message) final;

  const omnetpp::SimTime delay{1, omnetpp::SIMTIME_S};
  std::unique_ptr<omnetpp::cMessage> periodicTxMessage;
};

}  // namespace mocks
}  // namespace smile
