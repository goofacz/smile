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

#ifdef WITH_DECAWEAVE

#include "../utilities.h"
#include "deca_device_api.h"

extern "C" {

decaIrqStatus_t decamutexon()
{
  // TODO
}

void decamutexoff(decaIrqStatus_t s)
{
  // TODO
}

void deca_sleep(unsigned int time_ms)
{
  // TODO
}

int writetospi(uint16 headerLength, const uint8* headerBuffer, uint32 bodylength, const uint8* bodyBuffer)
{
  // TODO
}

int readfromspi(uint16 headerLength, const uint8* headerBuffer, uint32 readlength, uint8* readBuffer)
{
  // TODO
}

}  // extern "C"

namespace smile {

Define_Module(DecaWeaveNicDriver);

inet::MACAddress DecaWeaveNicDriver::getMacAddress() const
{
  dwt_getpartid();
  // TODO
}

void DecaWeaveNicDriver::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);
  // TODO
}

void DecaWeaveNicDriver::handleIncommingMessage(omnetpp::cMessage *newMessage)
{
  // TODO
}

void DecaWeaveNicDriver::receiveSignal(omnetpp::cComponent *source, omnetpp::simsignal_t signalID, long value,
                                       omnetpp::cObject *details)
{
  // TODO
}

}  // namespace smile

#endif  // WITH_DECAWEAVE
