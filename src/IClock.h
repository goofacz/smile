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

#include <omnetpp.h>
#include <experimental/optional>

namespace smile {

class IClock
{
 public:
  using OptionalSimTime = std::experimental::optional<omnetpp::SimTime>;

 public:
  IClock(const IClock& source) = delete;
  IClock(IClock&& source) = delete;
  virtual ~IClock() = default;

  IClock& operator=(const IClock& source) = delete;
  IClock& operator=(IClock&& source) = delete;

  virtual omnetpp::SimTime getClockTimestamp() = 0;
  virtual OptionalSimTime convertToSimulationTimestamp(const omnetpp::SimTime& timestamp) = 0;

  static const omnetpp::simsignal_t windowUpdateSignal;

 protected:
  IClock() = default;
};

}  // namespace smile
