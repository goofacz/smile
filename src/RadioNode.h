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

#include <functional>
#include "inet/common/geometry/common/Coord.h"
#include "inet/linklayer/common/MACAddress.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "omnetpp.h"

namespace smile {

class RadioNode : public omnetpp::cSimpleModule, public omnetpp::cListener
{
 public:
  using TxStateChangedCallback = std::function<void(inet::physicallayer::IRadio::TransmissionState)>;
  using RxStateChangedCallback = std::function<void(inet::physicallayer::IRadio::ReceptionState)>;

 public:
  RadioNode() = default;
  RadioNode(const RadioNode& source) = delete;
  RadioNode(RadioNode&& source) = delete;
  ~RadioNode() override = default;

  RadioNode& operator=(const RadioNode& source) = delete;
  RadioNode& operator=(RadioNode&& source) = delete;

  inet::MACAddress getMACAddress() const;
  const inet::Coord& getCurrentPosition() const;

  void addTxStateChangedCallback(TxStateChangedCallback callback);
  void addRxStateChangedCallback(RxStateChangedCallback callback);

 protected:
  void initialize(int stage) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override;

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, omnetpp::cObject* value,
                     omnetpp::cObject* details) override;

 private:
  int numInitStages() const override;

  void mobilityStateChangedCallback(omnetpp::cComponent* source, simsignal_t signalID, omnetpp::cObject* value,
                                    omnetpp::cObject* details);

  void txStateChangedCallback(inet::physicallayer::IRadio::TransmissionState state);

  void rxStateChangedCallback(inet::physicallayer::IRadio::ReceptionState state);

  std::vector<TxStateChangedCallback> txStateChangedcallbacks;
  std::vector<RxStateChangedCallback> rxStateChangedcallbacks;
  inet::Coord currentPosition;
};

}  // namespace smile
