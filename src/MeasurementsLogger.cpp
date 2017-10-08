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

#include <cassert>
#include "inet/common/INETDefs.h"

#include "MeasurementsLogger.h"

namespace smile {

Define_Module(MeasurementsLogger);

MeasurementsLogger::~MeasurementsLogger()
{
  logFile.flush();
}

MeasurementsLogger& MeasurementsLogger::operator<<(const Log& log)
{
  std::ostringstream buffer;
  return *this;
}

std::string MeasurementsLogger::to_string(FrameDirection direction)
{
  switch (direction) {
    case FrameDirection::TX:
      return "TX";
    case FrameDirection::RX:
      return "RX";
  }
}

void MeasurementsLogger::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_LOCAL) {
  }
}

int MeasurementsLogger::numInitStages() const
{
  return inet::INITSTAGE_LOCAL + 1;
}

void MeasurementsLogger::openFile()
{
  const auto& directoryPathParameter = par("directoryPath");
  const auto& fileNameParameter = par("fileName");
  const auto& formatParameter = par("format");
  const auto& overwriteParameter = par("overwrite");

  assert(directoryPathParameter.getType() == cPar::STRING);
  assert(fileNameParameter.getType() == cPar::STRING);
  assert(formatParameter.getType() == cPar::STRING);
  assert(overwriteParameter.getType() == cPar::BOOL);

  const auto directoryPath = directoryPathParameter.stdstringValue();
  if (directoryPath.empty()) {
    EV_ERROR << "MeasurementsLogger.directoryPath cannot be empty" << endl;
    endSimulation();
  }

  const auto fileName = fileNameParameter.stdstringValue();
  if (fileName.empty()) {
    EV_ERROR << "MeasurementsLogger.fileName cannot be empty" << endl;
    endSimulation();
  }

  const auto format = formatParameter.stdstringValue();
  if (format.empty()) {
    EV_ERROR << "MeasurementsLogger.format cannot be empty" << endl;
    endSimulation();
  }
  if (format != "CSV") {
    EV_ERROR << "MeasurementsLogger supports only CSV format" << endl;
    endSimulation();
  }

  const auto overwrite = overwriteParameter.boolValue();

  std::ostringstream filePath;
  filePath << directoryPath << '/' << fileName;
  const auto mode = std::ios_base::out | (overwrite ? std::ios_base::trunc : std::ios_base::ate);

  logFile = std::fstream(filePath.str(), mode);
  if (!logFile.is_open()) {
    EV_ERROR << "MeasurementsLogger failed to open " << filePath.str() << endl;
    endSimulation();
  }

  EV_INFO << "MeasurementsLogger will write logs to " << filePath.str() << endl;
}

}  // namespace smile
