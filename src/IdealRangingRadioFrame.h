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

#include "IdealRangingRadioFrame_m.h"

namespace smile {

class IdealRangingRadioFrame : public IdealRangingRadioFrame_Base
{
 public:
  IdealRangingRadioFrame(const char* name = nullptr, short kind = 0);
  IdealRangingRadioFrame(const IdealRangingRadioFrame& other);
  IdealRangingRadioFrame(IdealRangingRadioFrame&& other) = delete;
  IdealRangingRadioFrame& operator=(const IdealRangingRadioFrame& other);
  IdealRangingRadioFrame& operator=(IdealRangingRadioFrame&& other) = delete;

  IdealRangingRadioFrame* dup() const override;

  void setClockTimestamnp(const SimTime& newTimestamp);
  const SimTime& getClockTimestamp() const;

 private:
  void copy(const IdealRangingRadioFrame& other);

  SimTime timestamp{0};
};

}  // namespace smile
