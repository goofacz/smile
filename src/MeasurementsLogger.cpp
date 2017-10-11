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
#include <type_traits>
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
  std::string buffer{50};  // TODO Set better initial size
  buffer += std::to_string(log.frame.getSrcAddr().getInt());
  buffer += ",";
  buffer += std::to_string(log.frame.getDestAddr().getInt());
  buffer += ",";
  buffer += std::to_string(log.simulationTimestamp.inUnit(omnetpp::SIMTIME_AS));
  buffer += ",";
  buffer += std::to_string(log.nodeTimestamp.inUnit(omnetpp::SIMTIME_AS));
  buffer += ",";
  buffer += std::to_string(log.truePosition.x);
  buffer += ",";
  buffer += std::to_string(log.truePosition.y);
  buffer += ",";
  buffer += std::to_string(log.truePosition.z);
  buffer += ",";
  buffer += to_string(log.direction);
  buffer += ",";
  buffer += std::to_string(log.frame.getId());
  buffer += "\n";

  logFile << buffer;

  return *this;
}

std::string MeasurementsLogger::to_string(FrameDirection direction)
{
  switch (direction) {
    case FrameDirection::TX:
      return "TX";
    case FrameDirection::RX:
      return "RX";
    default:
      throw omnetpp::cRuntimeError("Unknown or invalid smile::MeasurementsLogger::FrameDirection value: %d",
                                   static_cast<std::underlying_type<decltype(direction)>::type>(direction));
  }
}

void MeasurementsLogger::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_LOCAL) {
    openFile();
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
    throw omnetpp::cRuntimeError{"MeasurementsLogger.directoryPath cannot be empty"};
  }

  const auto fileName = fileNameParameter.stdstringValue();
  if (fileName.empty()) {
    throw omnetpp::cRuntimeError{"MeasurementsLogger.fileName cannot be empty"};
  }

  const auto format = formatParameter.stdstringValue();
  if (format.empty()) {
    throw omnetpp::cRuntimeError{"MeasurementsLogger.format cannot be empty"};
  }
  if (format != "CSV") {
    throw omnetpp::cRuntimeError{"MeasurementsLogger supports only CSV format"};
  }

  const auto overwrite = overwriteParameter.boolValue();

  auto filePath{directoryPath};
  filePath += "/";  // TODO What about win32's '\'?
  filePath += fileName;
  const auto mode = std::ios_base::out | (overwrite ? std::ios_base::trunc : std::ios_base::ate);

  logFile = std::fstream(filePath, mode);
  if (!logFile.is_open()) {
    throw omnetpp::cRuntimeError{"MeasurementsLogger failed to open %s", filePath.c_str()};
  }

  EV_INFO << "MeasurementsLogger will write logs to " << filePath << endl;
}

MeasurementsLogger::Log::Log(const inet::MACFrameBase& initalFrame,
                             const omnetpp::SimTime& initialSimulationTimestamp,
                             const omnetpp::SimTime& initialNodeTimestamp,
                             const FrameDirection initialDirection, const inet::Coord& initialTruePosition)
    : frame{initalFrame},
      simulationTimestamp{initialSimulationTimestamp},
      nodeTimestamp{initialNodeTimestamp},
      direction{initialDirection},
      truePosition{initialTruePosition}
{}

}  // namespace smile
