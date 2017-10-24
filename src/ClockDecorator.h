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

#include <type_traits>
#include "IClock.h"
#include "omnetpp.h"

namespace smile {

template <typename BaseModule>
class ClockDecorator : public BaseModule
{
  static_assert(std::is_base_of<omnetpp::cModule, BaseModule>::value,
                "ClockDecorator has t derive from omnetpp::cModule");

 public:
  ClockDecorator() = default;
  ClockDecorator(const ClockDecorator& source) = delete;
  ClockDecorator(ClockDecorator&& source) = delete;
  ~ClockDecorator() override = default;

  ClockDecorator& operator=(const ClockDecorator& source) = delete;
  ClockDecorator& operator=(ClockDecorator&& source) = delete;

 protected:
  void initialize(int stage) override;

 private:
  IClock* clock{nullptr};
};

template <typename BaseModule>
void ClockDecorator<BaseModule>::initialize(int stage)
{
  BaseModule::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    // TODO
  }
}

}  // namespace smile
