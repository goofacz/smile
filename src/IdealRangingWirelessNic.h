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
#include <inet/physicallayer/contract/packetlevel/IRadio.h>
#include <omnetpp.h>
#include "ClockDecorator.h"
#include "IRangingWirelessNic.h"
#include "IdealRxCompletion_m.h"
#include "IdealTxCompletion_m.h"

namespace smile {

class IdealRangingWirelessNic : public ClockDecorator<omnetpp::cSimpleModule>, public IRangingWirelessNic
{
 public:
  IdealRangingWirelessNic() = default;
  IdealRangingWirelessNic(const IdealRangingWirelessNic& source) = delete;
  IdealRangingWirelessNic(IdealRangingWirelessNic&& source) = delete;
  ~IdealRangingWirelessNic() = default;

  IdealRangingWirelessNic& operator=(const IdealRangingWirelessNic& source) = delete;
  IdealRangingWirelessNic& operator=(IdealRangingWirelessNic&& source) = delete;

 protected:
  using ClockDecorator<omnetpp::cSimpleModule>::receiveSignal;

 private:
  void initialize(int stage) override final;

  void handleIncommingMessage(omnetpp::cMessage* newMessage) override final;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override final;

  void handleIdealIn(std::unique_ptr<inet::IdealMacFrame> frame);

  void handleNicIn(std::unique_ptr<inet::IdealMacFrame> frame);

  void handleRadioStateChanged(inet::physicallayer::IRadio::TransmissionState newState);

  void handleRadioStateChanged(inet::physicallayer::IRadio::ReceptionState newState);

  IdealTxCompletion txCompletion;
  IdealRxCompletion rxCompletion;
  inet::physicallayer::IRadio* radio{nullptr};
};

}  // namespace smile
