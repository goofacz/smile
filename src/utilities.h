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
#include <limits>
#include <memory>
#include <type_traits>

namespace smile {

template <typename OutputType, typename InputType>
auto dynamic_unique_ptr_cast(std::unique_ptr<InputType> object)
{
  omnetpp::check_and_cast<OutputType*>(object.get());
  return std::unique_ptr<OutputType>(static_cast<OutputType*>(object.release()));
};

template <typename T>
class SequenceNumberGenerator final
{
  static_assert(std::is_integral<T>::value, "T has to be an integral type");

 public:
  SequenceNumberGenerator() = default;
  SequenceNumberGenerator(const SequenceNumberGenerator&) = default;
  SequenceNumberGenerator(SequenceNumberGenerator&&) = default;
  ~SequenceNumberGenerator() = default;

  SequenceNumberGenerator& operator=(const SequenceNumberGenerator&) = default;
  SequenceNumberGenerator& operator=(SequenceNumberGenerator&&) = default;

  T generateNext()
  {
    if (current == std::numeric_limits<T>::max()) {
      throw omnetpp::cRuntimeError{"maximum sequence number reached"};
    }

    return current++;
  }

 private:
  T current{0};
};

}  // namespace smile
