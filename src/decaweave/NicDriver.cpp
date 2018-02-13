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

#include <cassert>

#include "NicDriver.h"

#ifdef WITH_DECAWEAVE

namespace smile {
namespace decaweave {

Define_Module(NicDriver);

const NicDriver::TransactionMap NicDriver::supportedTransactions = {
    {RegisterFile::DEV_ID, {Transaction::Operation::READ, {}}},
};

inet::MACAddress NicDriver::getMacAddress() const
{
  return inet::MACAddress{mac->par("address").stringValue()};
}

void NicDriver::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    const auto nicModulePath = par("nicModuleRelativePath").stringValue();
    auto nic = getModuleByPath(nicModulePath);
    if (!nic) {
      throw cRuntimeError{"Failed to find \"%s\" module", nicModulePath};
    }

    const auto macModulePath = ".mac";
    mac = nic->getModuleByPath(macModulePath);
    if (!mac) {
      throw cRuntimeError{"Failed to find \"%s\" module relative to \"nic\" module", macModulePath};
    }
  }
}

void NicDriver::handleIncommingMessage(omnetpp::cMessage* newMessage)
{
  // TODO
}

void NicDriver::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                              omnetpp::cObject* details)
{
  // TODO
}

NicDriver::Transaction::Transaction(Operation newOperation, std::vector<uint16_t> newSubaddress) :
    subaddresses{std::move(newSubaddress)}
{
  switch (newOperation) {
    case Operation::READ:
      readable = true;
      break;
    case Operation::WRITE:
      writable = true;
      break;
    case Operation::READ_WRITE:
      readable = true;
      writable = true;
      break;
  }
}

}  // namespace decaweave
}  // namespace smile

#endif  // WITH_DECAWEAVE
