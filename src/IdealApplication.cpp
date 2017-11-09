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

#include "IdealApplication.h"

#include <algorithm>
#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "utilities.h"

namespace smile {

Define_Module(IdealApplication);

void IdealApplication::initialize(int stage)
{
  cModule::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    measurementsLogger = inet::getModuleFromPar<MeasurementsLogger>(par("measurementsLoggerModule"), this);
  }
}

int IdealApplication::numInitStages() const
{
  return inet::INITSTAGE_APPLICATION_LAYER + 1;
}

void IdealApplication::initializeFrame(inet::MACFrameBase& frame, const inet::MACAddress& destinationAddress,
                                  const inet::MACAddress& sourceAddress)
{
  auto controlInformation = std::make_unique<inet::Ieee802Ctrl>();
  controlInformation->setSourceAddress(sourceAddress);
  controlInformation->setDest(destinationAddress);

  frame.setSrcAddr(sourceAddress);
  frame.setDestAddr(destinationAddress);
  frame.setControlInfo(controlInformation.release());
}

}  // namespace smile
