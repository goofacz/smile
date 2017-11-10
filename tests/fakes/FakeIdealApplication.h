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

#include "IdealApplication.h"

namespace smile {
namespace fakes {

class FakeIdealApplication : public IdealApplication
{
 public:
  FakeIdealApplication() = default;
  FakeIdealApplication(const FakeIdealApplication& source) = delete;
  FakeIdealApplication(FakeIdealApplication&& source) = delete;
  ~FakeIdealApplication() = default;

  FakeIdealApplication& operator=(const FakeIdealApplication& source) = delete;
  FakeIdealApplication& operator=(FakeIdealApplication&& source) = delete;

 private:
  void initialize(int stage) override;

  void handleSelfMessage(cMessage* newMessage) override;

  void handleIncommingMessage(cMessage* message) override;

  std::unique_ptr<omnetpp::cMessage> periodicTxMessage;
  unsigned int completedTxOperations{0};
};

}  // namespace fakes
}  // namespace smile
