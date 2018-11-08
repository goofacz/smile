//
// Copyright (C) 2018 Tomasz Jankowski <t.jankowski AT pwr.edu.pl>
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
#include <inet/linklayer/common/MacAddress.h>
#include <omnetpp.h>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <utility>
#include <vector>

namespace smile {

class Logger : public omnetpp::cSimpleModule
{
 private:
  enum class ExistingFilePolicy
  {
    ABORT,
    OVERWRITE,
    APPEND,
    PRESERVE
  };

 public:
  Logger() = default;
  Logger(const Logger& source) = delete;
  Logger(Logger&& source) = delete;
  ~Logger() override;

  Logger& operator=(const Logger& source) = delete;
  Logger& operator=(Logger&& source) = delete;

  void append(const std::string& entry);

 private:
  static ExistingFilePolicy stringToExistingFilePolicy(const std::string& value);

  void initialize(int stage) override;

  ExistingFilePolicy getExistingFilePolicy() const;

  std::filesystem::path createDirectory() const;
  void openFile(const std::filesystem::path& directoryPath);

  ExistingFilePolicy existingFilePolicy{ExistingFilePolicy::ABORT};
  std::filesystem::path filePath;
  std::ofstream logStream;
};

}  // namespace smile
