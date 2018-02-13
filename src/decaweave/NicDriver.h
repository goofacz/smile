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

#include <inet/linklayer/ideal/IdealMacFrame_m.h>
#include <inet/mobility/contract/IMobility.h>
#include <inet/physicallayer/contract/packetlevel/IRadio.h>
#include <omnetpp.h>
#include <map>
#include "../ClockDecorator.h"
#include "../IRangingNicDriver.h"
#include "../IdealRxCompletion_m.h"
#include "../IdealTxCompletion_m.h"

namespace smile {
namespace decaweave {

class NicDriver : public ClockDecorator<omnetpp::cSimpleModule>, public IRangingNicDriver
{
 private:
  enum class Operation : uint8_t
  {
    READ = 0x0,
    WRITE = 0x1,
    READ_WRITE = 0x10
  };

  enum class RegisterFile : uint8_t
  {
    DEV_ID = 0x00
  };

  struct Transaction
  {
    enum class Operation
    {
      READ,
      WRITE,
      READ_WRITE
    };

    Transaction(Operation newOperation, std::vector<uint16_t> newSubaddress);

    bool readable{false};
    bool writable{false};
    std::vector<uint16_t> subaddresses;
  };

  using TransactionMap = std::map<RegisterFile, Transaction>;

 public:
  NicDriver() = default;
  NicDriver(const NicDriver& source) = delete;
  NicDriver(NicDriver&& source) = delete;
  ~NicDriver() = default;

  NicDriver& operator=(const NicDriver& source) = delete;
  NicDriver& operator=(NicDriver&& source) = delete;

  inet::MACAddress getMacAddress() const override;

 private:
  void initialize(int stage) override final;

  void handleIncommingMessage(omnetpp::cMessage* newMessage) override final;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override final;

  static const TransactionMap supportedTransactions;

  cModule* mac{nullptr};
};

}  // namespace decaweave
}  // namespace smile

#endif  // WITH_DECAWEAVE
