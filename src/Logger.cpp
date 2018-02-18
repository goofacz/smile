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
  if (!logStream.is_open()) {
    return;
  }

  logStream.exceptions(std::ifstream::goodbit);
  logStream.flush();
}

void Logger::append(const std::string& entry)
{
  logStream << entry;
  if (entry.back() != '\n') {
    logStream << "\n";
  }
}

Logger::ExistingFilePolicy Logger::stringToExistingFilePolicy(const std::string value)
{
  if (value == "abort") {
    return ExistingFilePolicy::ABORT;
  }
  else if (value == "overwrite") {
    return ExistingFilePolicy::OVERWRITE;
  }
  else if (value == "append") {
    return ExistingFilePolicy::APPEND;
  }
  else {
    throw cRuntimeError{"Invalid Logger's \"existingFilePolicy\" parameter value: %s", value.c_str()};
  }
}

void Logger::initialize(int stage)
{
  cSimpleModule::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    const auto directoryPath = createDirectory();
    logStream = openFile(directoryPath);
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

std::ofstream Logger::openFile(const std::experimental::filesystem::path& directoryPath)
{
  using namespace std::experimental;
  try {
    // Prepare & verify file path
    const auto existingFilePolicy = stringToExistingFilePolicy(par("existingFilePolicy").stdstringValue());
    const auto fileName = par("fileName").stdstringValue();
    if(fileName.empty())    {
      throw cRuntimeError{"Logger property \"fileName\" cannot be empty"};
    }

    auto filePath = directoryPath;
    filePath.append(fileName);
    if (filesystem::exists(filePath) && existingFilePolicy == ExistingFilePolicy::ABORT) {
      throw cRuntimeError{"Log file \"%s\" already exists, aborting simulation", filePath.c_str()};
    }

    // Open file
    std::ofstream logFile;
    logFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    auto mode = existingFilePolicy == ExistingFilePolicy::OVERWRITE ? std::ios_base::trunc : std::ios_base::ate;
    mode |= std::ios_base::out;
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

}  // namespace smile
