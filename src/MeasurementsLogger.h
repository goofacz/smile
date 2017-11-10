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
#include <inet/linklayer/base/MACFrameBase_m.h>
#include <omnetpp.h>
#include <fstream>
#include <memory>

namespace smile {

class MeasurementsLogger : public omnetpp::cSimpleModule
{
 public:
  enum class FrameDirection
  {
    TX,
    RX
  };

  struct Log
  {
    Log(const inet::MACFrameBase& initalFrame, const SimTime& initialSimulationTimestamp,
        const SimTime& initialNodeTimestamp, const FrameDirection initialDirection,
        const inet::Coord& initialTruePosition);

    const inet::MACFrameBase& frame;
    const SimTime simulationTimestamp;
    const SimTime clockTimestamp;
    const FrameDirection direction;
    const inet::Coord truePosition;
  };

 public:
  MeasurementsLogger() = default;
  MeasurementsLogger(const MeasurementsLogger& source) = delete;
  MeasurementsLogger(MeasurementsLogger&& source) = delete;
  ~MeasurementsLogger() override;

  MeasurementsLogger& operator=(const MeasurementsLogger& source) = delete;
  MeasurementsLogger& operator=(MeasurementsLogger&& source) = delete;

  MeasurementsLogger& operator<<(const Log& log);

 private:
  static std::string to_string(FrameDirection direction);

  void initialize(int stage) override;

  int numInitStages() const override;

  void openFile();

  std::fstream logFile;
};

}  // namespace smile
