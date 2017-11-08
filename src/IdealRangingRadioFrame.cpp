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

#include "IdealRangingRadioFrame.h"

namespace smile {

Register_Class(IdealRangingRadioFrame);

IdealRangingRadioFrame::IdealRangingRadioFrame(const char* name, short kind) : IdealRangingRadioFrame_Base(name, kind)
{}

IdealRangingRadioFrame::IdealRangingRadioFrame(const IdealRangingRadioFrame& other) : IdealRangingRadioFrame_Base(other)
{
  copy(other);
}

IdealRangingRadioFrame& IdealRangingRadioFrame::operator=(const IdealRangingRadioFrame& other)
{
  if (this == &other) {
    return *this;
  }

  IdealRangingRadioFrame_Base::operator=(other);
  copy(other);

  return *this;
}

IdealRangingRadioFrame* IdealRangingRadioFrame::dup() const
{
  return new IdealRangingRadioFrame(*this);
}

void IdealRangingRadioFrame::copy(const IdealRangingRadioFrame& other)
{
  timestamp = other.timestamp;
}

void IdealRangingRadioFrame::setClockTimestamnp(const SimTime& newTimestamp)
{
  timestamp = newTimestamp;
}

const SimTime& IdealRangingRadioFrame::getClockTimestamp() const
{
  return timestamp;
}

}  // namespace smile
