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

#include <inet/mobility/contract/IMobility.h>
#include <inet/physicallayer/contract/packetlevel/IRadio.h>
#include <omnetpp.h>
#include "ClockDecorator.h"
#include "Logger.h"

namespace smile {

class ApplicationBase : public ClockDecorator<omnetpp::cSimpleModule>
{
 public:
  ApplicationBase() = default;
  ApplicationBase(const ApplicationBase& source) = delete;
  ApplicationBase(ApplicationBase&& source) = delete;
  ~ApplicationBase() = default;

  ApplicationBase& operator=(const ApplicationBase& source) = delete;
  ApplicationBase& operator=(ApplicationBase&& source) = delete;

 protected:
  using ClockDecorator<omnetpp::cSimpleModule>::receiveSignal;

  void initialize(int stage) override;

  inet::physicallayer::IRadio& getRadio();
  Logger& getLlogger();
  inet::IMobility& getMobility();

 private:
  int numInitStages() const final;

  inet::physicallayer::IRadio* radio{nullptr};
  Logger* logger{nullptr};
  inet::IMobility* mobility{nullptr};
};

}  // namespace smile
