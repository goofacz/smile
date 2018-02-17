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

#include <map>
#include <vector>
#include "../Application.h"
#include "deca_device_api.h"

namespace smile {
namespace decaweave {

extern "C" {

extern int readfromspi(uint16 headerLength, const uint8* headerBuffer, uint32 readlength, uint8* readBuffer);

extern int writetospi(uint16 headerLength, const uint8* headerBuffer, uint32 bodylength, const uint8* bodyBuffer);

}  // extern "C"

class Application : public smile::Application
{
  friend int readfromspi(uint16 headerLength, const uint8* headerBuffer, uint32 readlength, uint8* readBuffer);
  friend int writetospi(uint16 headerLength, const uint8* headerBuffer, uint32 bodylength, const uint8* bodyBuffer);
  friend class CurrentApplicationGuard;

 private:
  enum class Operation
  {
    READ,
    WRITE,
  };

  using RegisterFile = uint8_t;
  using RegisterOffset = uint16_t;
  using FullRegisterFile = std::pair<RegisterFile, RegisterOffset>;
  using RegisterFileMap = std::map<RegisterFile, std::vector<uint8_t>>;

 public:
  Application();
  Application(const Application& source) = delete;
  Application(Application&& source) = delete;
  ~Application() = default;

  Application& operator=(const Application& source) = delete;
  Application& operator=(Application&& source) = delete;

 protected:
  void initialize(int stage) override;

 private:
  static unsigned int generateDecaLibIndex();

  void handleIncommingMessage(cMessage* message) override final;

  int decodeTransaction(uint16_t headerLength, const uint8_t* headerBuffer, uint32_t dataLength, uint8_t* readBuffer,
                        const uint8_t* writeBuffer);

  int handleReadTransaction(const FullRegisterFile& fullRegisterFile, uint32_t readLength, uint8_t* readBuffer);

  int handleWriteTransaction(const FullRegisterFile& fullRegisterFile, uint32_t writeLength,
                             const uint8_t* writeBuffer);

  unsigned int getDecaLibIndex() const;

  void resetRegisterFiles();

  const unsigned int decaLibIndex;
  RegisterFileMap registerFiles;
};

}  // namespace decaweave
}  // namespace smile

#endif  // WITH_DECAWEAVE
