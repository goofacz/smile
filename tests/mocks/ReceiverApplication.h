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

class ReceiverApplication final : public Application
{
 public:
  ReceiverApplication() = default;
  ReceiverApplication(const ReceiverApplication& source) = delete;
  ReceiverApplication(ReceiverApplication&& source) = delete;
  ~ReceiverApplication() = default;

  ReceiverApplication& operator=(const ReceiverApplication& source) = delete;
  ReceiverApplication& operator=(ReceiverApplication&& source) = delete;

 private:
  void handleReceptionCompleted(std::unique_ptr<inet::MACFrameBase> frame,
                                const omnetpp::SimTime& clockTimestamp) override final;
};

}  // namespace mocks
}  // namespace smile
