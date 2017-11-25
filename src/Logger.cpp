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

#include "Logger.h"
#include <inet/common/INETDefs.h>
#include <algorithm>

namespace smile {

Define_Module(Logger);

Logger::~Logger()
{
  for (auto& logFile : logFiles) {
    auto& logStream = logFile.second;
    logStream.exceptions(std::ifstream::goodbit);
    logStream.flush();
  }
}

Logger::Handle Logger::obtainHandle(const std::string& name)
{
  auto predicate = [name](auto& logFile) { return logFile.first == name; };
  auto foundLogFile = std::find_if(logFiles.begin(), logFiles.end(), predicate);
  if (foundLogFile != logFiles.end()) {
    return {std::distance(logFiles.begin(), foundLogFile)};
  }

  auto newLogFile = logFiles.emplace(logFiles.end(), name, openFile(name));
  return {std::distance(logFiles.begin(), newLogFile)};
}

void Logger::append(const Handle& handle, const std::string& entry)
{
  try {
    auto& logFile = logFiles.at(handle.index);
    auto& logStream = logFile.second;
    logStream << entry;
    if (entry.back() != '\n') {
      logStream << "\n";
    }
  }
  catch (const std::out_of_range&) {
    throw cRuntimeError("Failed to find log file registered at index: %d", handle.index);
  }
}

std::experimental::filesystem::path Logger::createDirectory() const
{
  using namespace std::experimental;

  filesystem::path path{par("directoryPath").stdstringValue()};

  try {
    path.make_preferred();

    if (!filesystem::exists(path)) {
      std::error_code errorCode;
      const auto result = filesystem::create_directories(path, errorCode);
      if (!result) {
        throw cRuntimeError{"Cannot create directory \"%s\": %s", path.c_str(), errorCode.message().c_str()};
      }
    }

    if (!filesystem::is_directory(path)) {
      throw cRuntimeError{"Cannot open log file, \"%s\" is not a directory", path.c_str()};
    }

    return path;
  }
  catch (const filesystem::filesystem_error& error) {
    throw cRuntimeError{"Failed to create or open directory \"%s\": %s", path.c_str(), error.what()};
  }
}

std::string Logger::composeFileName(const std::string& middlePart) const
{
  std::string buffer{par("fileNamePrefix").stdstringValue()};
  buffer += "_";
  buffer += middlePart;
  buffer += ".";
  buffer += par("extension").stdstringValue();
  return buffer;
}

std::ofstream Logger::openFile(const std::string& name)
{
  using namespace std::experimental;
  try {
    // Prepare & verify file path
    const auto overwrite = par("overwrite").boolValue();
    const auto fileName = composeFileName(name);
    auto filePath = createDirectory();
    filePath.append(fileName);
    if (filesystem::exists(filePath)) {
      if (!filesystem::is_regular_file(filePath)) {
        throw cRuntimeError{"Log file \"%s\" already exists and it is not a regular file", filePath.c_str()};
      }

      if (filesystem::file_size(filePath) > 0 && !overwrite) {
        throw cRuntimeError{"Log file \"%s\" has content and cannot be overwritten", filePath.c_str()};
      }
    }

    // Open file
    std::ofstream logFile;
    logFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    const auto mode = std::ios_base::out | (overwrite ? std::ios_base::trunc : std::ios_base::ate);
    logFile.open(filePath, mode);

    return logFile;
  }
  catch (const filesystem::filesystem_error& error) {
    throw cRuntimeError{"Failed to create or open log file: %s", error.what()};
  }
  catch (const std::ios_base::failure& error) {
    throw cRuntimeError{"Failed to open log file: %s", error.what()};
  }
}

Logger::Handle::Handle(Index initialIndex) : index{initialIndex} {}

}  // namespace smile
