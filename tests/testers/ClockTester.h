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

#include "ClockDecorator.h"

namespace smile {
namespace testers {

class ClockTester : public ClockDecorator<omnetpp::cSimpleModule>
{
 public:
  ClockTester() = default;
  ClockTester(const ClockTester& source) = delete;
  ClockTester(ClockTester&& source) = delete;
  ~ClockTester() = default;

  ClockTester& operator=(const ClockTester& source) = delete;
  ClockTester& operator=(ClockTester&& source) = delete;

 private:
  void initialize(int stage) override;

  int numInitStages() const override;

  IClock* clock{nullptr};
};

}  // namespace testers
}  // namespace smile
