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
#include <experimental/filesystem>
#include <fstream>
#include <iterator>
#include <utility>
#include <vector>

namespace smile {

namespace csv_logger {

template <typename T>
struct Converter
{};

template <>
struct Converter<char*>
{
  static std::string convert(const char* element)
  {
    return element;
  }
};

template <>
struct Converter<inet::Coord>
{
  static std::string convert(const inet::Coord& element)
  {
    std::string buffer;
    buffer += std::to_string(element.x);
    buffer += ";";
    buffer += std::to_string(element.y);
    buffer += ";";
    buffer += std::to_string(element.z);
    return buffer;
  }
};

template <>
struct Converter<inet::MACAddress>
{
  static std::string convert(const inet::MACAddress& element)
  {
    return std::to_string(element.getInt());
  }
};

template <>
struct Converter<omnetpp::SimTime>
{
  static std::string convert(const omnetpp::SimTime& element)
  {
    return std::to_string(element.inUnit(omnetpp::SIMTIME_FS));
  }
};

};  // namespace csv_logger

class Logger : public omnetpp::cSimpleModule
{
 private:
  using LogFile = std::pair<std::string, std::ofstream>;
  using LogFileVector = std::vector<LogFile>;

  template <int size, typename T, typename... Arguments>
  struct CsvHelper
  {
    static std::string compose(std::string buffer, T&& element, Arguments&&... arguments)
    {
      using NakedT = typename std::decay<T>::type;
      buffer += csv_logger::Converter<NakedT>::convert(std::forward<T>(element));
      buffer += ";";
      return CsvHelper<sizeof...(arguments) - 1, Arguments...>::compose(std::move(buffer),
                                                                        std::forward<Arguments>(arguments)...);
    }
  };

  template <typename T>
  struct CsvHelper<0, T>
  {
    static std::string compose(std::string buffer, T&& element)
    {
      using NakedT = typename std::decay<T>::type;
      buffer += csv_logger::Converter<NakedT>::convert(std::forward<T>(element));
      return buffer;
    }
  };

 public:
  class Handle final
  {
    friend Logger;

   private:
    using Index = std::iterator_traits<LogFileVector::iterator>::difference_type;

   public:
    Handle() = default;
    Handle(const Handle& source) = default;
    Handle(Handle&& source) = default;
    ~Handle() = default;

    Handle& operator=(const Handle& source) = default;
    Handle& operator=(Handle&& source) = default;

   private:
    Handle(Index initialIndex);
    Index index{-1};
  };

 public:
  Logger() = default;
  Logger(const Logger& source) = delete;
  Logger(Logger&& source) = delete;
  ~Logger() override;

  Logger& operator=(const Logger& source) = delete;
  Logger& operator=(Logger&& source) = delete;

  template <typename... Arguments>
  static std::string composeCsvEntry(Arguments&&... arguments);

  Handle obtainHandle(const std::string& name);
  void append(const Handle& handle, const std::string& entry);

 private:
  std::experimental::filesystem::path createDirectory() const;
  std::string composeFileName(const std::string& middlePart) const;
  std::ofstream openFile(const std::string& name);

  LogFileVector logFiles;
};

template <typename... Arguments>
std::string Logger::composeCsvEntry(Arguments&&... arguments)
{
  return CsvHelper<sizeof...(arguments) - 1, Arguments...>::compose({}, std::forward<Arguments>(arguments)...);
}

}  // namespace smile
