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

#include "../smile_features.h"
#ifdef WITH_DECAWEAVE

#include <inet/common/geometry/common/Coord.h>
#include <inet/mobility/contract/IMobility.h>
#include <omnetpp.h>
#include "../ClockDecorator.h"
#include "../IApplication.h"
#include "../IRangingNicDriver.h"
#include "../Logger.h"

namespace smile {
namespace decaweave {

class Application : public ClockDecorator<omnetpp::cSimpleModule>, public IApplication
{
 public:
  Application() = default;
  Application(const Application& source) = delete;
  Application(Application&& source) = delete;
  ~Application() = default;

  Application& operator=(const Application& source) = delete;
  Application& operator=(Application&& source) = delete;

 protected:
  void initialize(int stage) override;
};

}  // namespace decaweave
}  // namespace smile

#endif  // WITH_DECAWEAVE
