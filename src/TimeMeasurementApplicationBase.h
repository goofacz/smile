//
// Copyright (C) 2018 Tomasz Jankowski <t.jankowski AT pwr.edu.pl>
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

#include "ApplicationBase.h"

namespace smile {

class TimeMeasurementApplicationBase : public ApplicationBase
{
 public:
  TimeMeasurementApplicationBase() = default;
  TimeMeasurementApplicationBase(const TimeMeasurementApplicationBase& source) = delete;
  TimeMeasurementApplicationBase(TimeMeasurementApplicationBase&& source) = delete;
  ~TimeMeasurementApplicationBase() = default;

  TimeMeasurementApplicationBase& operator=(const TimeMeasurementApplicationBase& source) = delete;
  TimeMeasurementApplicationBase& operator=(TimeMeasurementApplicationBase&& source) = delete;

 protected:
  void initialize(int stage) override;

  virtual void onTxOperationBegin();
  virtual void onRxOperationBegin();

 private:
  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                     omnetpp::cObject* details) override;

  inet::physicallayer::IRadio::TransmissionState txCurrentState{inet::physicallayer::IRadio::TransmissionState::TRANSMISSION_STATE_UNDEFINED};
  inet::physicallayer::IRadio::ReceptionState rxCurrentState{inet::physicallayer::IRadio::ReceptionState::RECEPTION_STATE_UNDEFINED};
};

}  // namespace smile
