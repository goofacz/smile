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

#include "applications/TimeMeasurementApplicationBase.h"
#include "project_features.h"

#ifdef WITH_SAMPLES

namespace smile::samples::ping_pong {

class PingPongApplication : public applications::TimeMeasurementApplicationBase
{
 public:
  PingPongApplication() = default;
  PingPongApplication(const PingPongApplication& source) = delete;
  PingPongApplication(PingPongApplication&& source) = delete;
  ~PingPongApplication() = default;

  PingPongApplication& operator=(const PingPongApplication& source) = delete;
  PingPongApplication& operator=(PingPongApplication&& source) = delete;

 protected:
  void initialize(int stage) override;

  void onTxOperationBegin(const inet::physicallayer::ITransmission* transmision) override;
  void onRxOperationBegin(const inet::physicallayer::ITransmission* reception) override;
};

}  // namespace smile::samples::ping_pong

#endif
