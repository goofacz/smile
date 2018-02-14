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

#include "../Application.h"
#include "deca_device_api.h"

namespace smile {
namespace decaweave {

extern "C" {

extern int readfromspi(uint16 headerLength, const uint8* headerBuffer, uint32 readlength, uint8* readBuffer);

}  // extern "C"

class Application : public smile::Application
{
  friend int readfromspi(uint16 headerLength, const uint8* headerBuffer, uint32 readlength, uint8* readBuffer);

 private:
  enum class Operation
  {
    READ,
    WRITE,
  };

  using RegisterFile = uint8_t;

 public:
  Application() = default;
  Application(const Application& source) = delete;
  Application(Application&& source) = delete;
  ~Application() = default;

  Application& operator=(const Application& source) = delete;
  Application& operator=(Application&& source) = delete;

 protected:
  void initialize(int stage) override;

 private:
  void handleIncommingMessage(cMessage* message) override final;

  int decodeTransaction(uint16_t headerLength, const uint8_t* headerBuffer, uint32_t readlength, uint8_t* readBuffer);

  int handleReadDevId(uint32_t readlength, uint8_t* readBuffer);
};

}  // namespace decaweave
}  // namespace smile

#endif  // WITH_DECAWEAVE
