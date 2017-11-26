//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

cplusplus	{{
#include <inet/linklayer/ideal/IdealMacFrame_m.h>
#include <inet/common/geometry/common/Coord.h>
#include "IRangingNicDriver.h"

using IdealMacFramePointer = const inet::IdealMacFrame*;
}}

namespace smile;

class noncobject IRangingNicDriver::IdealTxCompletionStatus;
class noncobject IdealMacFramePointer;
class noncobject inet::Coord;

class IdealTxCompletion {
    IRangingNicDriver::IdealTxCompletionStatus status = IRangingNicDriver::IdealTxCompletionStatus::SUCCESS;
    IdealMacFramePointer frame = nullptr;
    simtime_t operationBeginClockTimestamp = 0;
    simtime_t operationBeginSimulationTimestamp = 0;
    simtime_t operationEndClockTimestamp = 0;
    simtime_t operationEndSimulationTimestamp = 0;
    inet::Coord operationBeginTruePosition;
    inet::Coord operationEndTruePosition;
}
