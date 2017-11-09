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
#include "ClockDecorator.h"
#include "MeasurementsLogger.h"
#include "inet/linklayer/base/MACFrameBase_m.h"
#include "omnetpp.h"

namespace smile {

class IdealApplication : public ClockDecorator<omnetpp::cSimpleModule>, public IApplication
{
 public:
  IdealApplication() = default;
  IdealApplication(const IdealApplication& source) = delete;
  IdealApplication(IdealApplication&& source) = delete;
  ~IdealApplication() = default;

  IdealApplication& operator=(const IdealApplication& source) = delete;
  IdealApplication& operator=(IdealApplication&& source) = delete;

 protected:
  void initialize(int stage) override;

  template <typename Frame, typename... FrameArguments>
  std::unique_ptr<Frame> createFrame(const inet::MACAddress& destinationAddress, FrameArguments&&... frameArguments);

 private:
  static void initializeFrame(inet::MACFrameBase& frame, const inet::MACAddress& destinationAddress,
                              const inet::MACAddress& sourceAddress);

  int numInitStages() const final;

  MeasurementsLogger* measurementsLogger{nullptr};
};

template <typename Frame, typename... FrameArguments>
std::unique_ptr<Frame> IdealApplication::createFrame(const inet::MACAddress& destinationAddress,
                                                FrameArguments&&... frameArguments)
{
  constexpr auto isDerived = std::is_base_of<inet::MACFrameBase, Frame>::value;
  constexpr auto isSame = std::is_same<inet::MACFrameBase, Frame>::value;
  static_assert(isDerived || isSame,
                "Application::scheduleFrameTransmission requires Frame to derive from inet::MACFrameBase");

  auto frame = std::make_unique<Frame>(std::forward<FrameArguments>(frameArguments)...);
  // const auto& localAddress = nic->getMacAddress();
  // initializeFrame(*frame, destinationAddress, localAddress);
  // return frame;
}

}  // namespace smile
