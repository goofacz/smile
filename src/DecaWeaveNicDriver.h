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

#include <inet/linklayer/ideal/IdealMacFrame_m.h>
#include <inet/mobility/contract/IMobility.h>
#include <inet/physicallayer/contract/packetlevel/IRadio.h>
#include <omnetpp.h>
#include "ClockDecorator.h"
#include "IRangingNicDriver.h"
#include "IdealRxCompletion_m.h"
#include "IdealTxCompletion_m.h"

namespace smile {

class DecaWeaveNicDriver : public ClockDecorator<omnetpp::cSimpleModule>, public IRangingNicDriver
{
 public:
  DecaWeaveNicDriver() = default;
  DecaWeaveNicDriver(const DecaWeaveNicDriver& source) = delete;
  DecaWeaveNicDriver(DecaWeaveNicDriver&& source) = delete;
  ~DecaWeaveNicDriver() = default;

  DecaWeaveNicDriver& operator=(const DecaWeaveNicDriver& source) = delete;
  DecaWeaveNicDriver& operator=(DecaWeaveNicDriver&& source) = delete;

  inet::MACAddress getMacAddress() const override;

 protected:
  using ClockDecorator<omnetpp::cSimpleModule>::receiveSignal;

 private:
  void initialize(int stage) override final;

  void handleIncommingMessage(omnetpp::cMessage* newMessage) override final;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override final;
};

}  // namespace smile
