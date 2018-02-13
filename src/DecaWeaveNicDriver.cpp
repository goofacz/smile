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

#include "DecaWeaveNicDriver.h"
#include "utilities.h"

namespace smile {

Define_Module(DecaWeaveNicDriver);

inet::MACAddress DecaWeaveNicDriver::getMacAddress() const
{
  // TODO
}

void DecaWeaveNicDriver::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);
  // TODO
}

void DecaWeaveNicDriver::handleIncommingMessage(omnetpp::cMessage* newMessage)
{
  // TODO
}

void DecaWeaveNicDriver::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                                       omnetpp::cObject* details)
{
  // TODO
}

}  // namespace smile
