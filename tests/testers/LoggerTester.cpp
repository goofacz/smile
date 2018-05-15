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

#include "LoggerTester.h"
#include <inet/common/ModuleAccess.h>
#include "IClock.h"

namespace smile {
namespace testers {

Define_Module(LoggerTester);

void LoggerTester::initialize(int stage)
{
  cSimpleModule::initialize(stage);

  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    // TODO
  }
}

int LoggerTester::numInitStages() const
{
  return inet::INITSTAGE_APPLICATION_LAYER + 1;
}

}  // namespace testers
}  // namespace smile
