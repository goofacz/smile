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

class Logger : public omnetpp::cSimpleModule
{
 private:
  using LogFile = std::pair<std::string, std::ofstream>;
  using LogFileVector = std::vector<LogFile>;

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

  Handle obtainHandle(const std::string& name);
  void append(const Handle& handle, const std::string& entry);

 private:
  std::experimental::filesystem::path createDirectory() const;
  std::string composeFileName(const std::string& middlePart) const;
  std::ofstream openFile(const std::string& name);

  LogFileVector logFiles;
};

}  // namespace smile
