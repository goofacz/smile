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

#include <inet/common/geometry/common/Coord.h>
#include <inet/linklayer/common/MACAddress.h>
#include <omnetpp.h>
#include <string>
#include <utility>
#include "IdealRxCompletion_m.h"
#include "IdealTxCompletion_m.h"

namespace smile {
namespace csv_logger {

template <typename T>
struct Converter
{
  static std::string convert(std::string buffer, T&& element)
  {
    buffer += std::to_string(std::forward<T>(element));
    return buffer;
  }
};

template <>
struct Converter<const char*>
{
  static std::string convert(std::string buffer, const char* element)
  {
    buffer += element;
    return buffer;
  }
};

template <>
struct Converter<std::string>
{
  static std::string convert(std::string buffer, const std::string& element)
  {
    buffer += element;
    return buffer;
  }
};

template <>
struct Converter<inet::Coord>
{
  static std::string convert(std::string buffer, const inet::Coord& element)
  {
    buffer += std::to_string(element.x);
    buffer += ",";
    buffer += std::to_string(element.y);
    buffer += ",";
    buffer += std::to_string(element.z);
    return buffer;
  }
};

template <>
struct Converter<inet::MACAddress>
{
  static std::string convert(std::string buffer, const inet::MACAddress& element)
  {
    buffer += std::to_string(element.getInt());
    return buffer;
  }
};

template <>
struct Converter<omnetpp::SimTime>
{
  static std::string convert(std::string buffer, const omnetpp::SimTime& element)
  {
    buffer += std::to_string(element.inUnit(omnetpp::SIMTIME_PS));
    return buffer;
  }
};

template <int argumentsNumber, typename T, typename... Arguments>
struct Composer
{
  static std::string compose(std::string buffer, T&& element, Arguments&&... arguments)
  {
    using NakedT = typename std::decay<T>::type;
    buffer = Composer<0, NakedT>::compose(std::move(buffer), std::forward<T>(element));
    buffer += ",";
    return Composer<sizeof...(arguments) - 1, Arguments...>::compose(std::move(buffer),
                                                                     std::forward<Arguments>(arguments)...);
  }
};

template <typename T>
struct Composer<0, T>
{
  static std::string compose(std::string buffer, T&& element)
  {
    using NakedT = typename std::decay<T>::type;
    return Converter<NakedT>::convert(std::move(buffer), std::forward<T>(element));
  }
};

template <typename... Arguments>
std::string compose(Arguments... arguments)
{
  return Composer<sizeof...(arguments) - 1, Arguments...>::compose("", std::forward<Arguments>(arguments)...);
}

template <typename... Arguments>
std::string composeWithBuffer(std::string buffer, Arguments... arguments)
{
  return Composer<sizeof...(arguments) - 1, Arguments...>::compose(std::move(buffer),
                                                                   std::forward<Arguments>(arguments)...);
}

template <>
struct Composer<0, IdealRxCompletion>
{
  static std::string compose(std::string buffer, const IdealRxCompletion& element)
  {
    const auto& frame = element.getFrame();
    return composeWithBuffer(std::move(buffer), "RX", element.getOperationBeginClockTimestamp(),
                             element.getOperationBeginSimulationTimestamp(), element.getOperationBeginTruePosition(),
                             element.getOperationEndClockTimestamp(), element.getOperationEndSimulationTimestamp(),
                             element.getOperationEndTruePosition(), frame->getSrc(), frame->getDest());
  }
};

template <>
struct Composer<0, IdealTxCompletion>
{
  static std::string compose(std::string buffer, const IdealTxCompletion& element)
  {
    const auto& frame = element.getFrame();
    return composeWithBuffer(std::move(buffer), "TX", element.getOperationBeginClockTimestamp(),
                             element.getOperationBeginSimulationTimestamp(), element.getOperationBeginTruePosition(),
                             element.getOperationEndClockTimestamp(), element.getOperationEndSimulationTimestamp(),
                             element.getOperationEndTruePosition(), frame->getSrc(), frame->getDest());
  }
};

}  // namespace csv_logger
}  // namespace smile
