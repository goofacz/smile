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

#include <memory>
#include "inet/linklayer/base/MACFrameBase_m.h"
#include "omnetpp.h"

namespace smile {

class FrameTuple : public omnetpp::cObject
{
 public:
  FrameTuple() = default;
  FrameTuple(std::unique_ptr<inet::MACFrameBase> newFrame, const omnetpp::SimTime& newTimestamp);
  FrameTuple(const FrameTuple& source) = default;
  FrameTuple(FrameTuple&& source) = default;
  ~FrameTuple() = default;

  FrameTuple& operator=(const FrameTuple& source) = default;
  FrameTuple& operator=(FrameTuple&& source) = default;

 private:
  std::unique_ptr<inet::MACFrameBase> frame;
  omnetpp::SimTime timestamp;
};

}  // namespace smile
