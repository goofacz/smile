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

#include "Application.h"

#ifdef WITH_DECAWEAVE

#include <inet/common/ModuleAccess.h>
#include <cassert>
#include "deca_regs.h"

namespace smile {
namespace decaweave {

class ApplicationSingleton final
{
 public:
  friend class CurrentApplicationGuard;

  ApplicationSingleton(const ApplicationSingleton& source) = delete;
  ApplicationSingleton(ApplicationSingleton&& source) = delete;
  ~ApplicationSingleton() = default;

  ApplicationSingleton& operator=(const ApplicationSingleton& source) = delete;
  ApplicationSingleton& operator=(ApplicationSingleton&& source) = delete;

  Application* operator->();

  static ApplicationSingleton& getInstance();

 private:
  ApplicationSingleton() = default;

  static ApplicationSingleton instance;

  Application* application{nullptr};
};

class CurrentApplicationGuard final
{
 public:
  explicit CurrentApplicationGuard(Application* currentApplication);
  CurrentApplicationGuard(const CurrentApplicationGuard& source) = delete;
  CurrentApplicationGuard(CurrentApplicationGuard&& source) = delete;
  ~CurrentApplicationGuard();

  CurrentApplicationGuard& operator=(const CurrentApplicationGuard& source) = delete;
  CurrentApplicationGuard& operator=(CurrentApplicationGuard&& source) = delete;
};

ApplicationSingleton ApplicationSingleton::instance;

Define_Module(Application);

void Application::initialize(int stage)
{
  smile::Application::initialize(stage);
  // TODO
}

void Application::handleIncommingMessage(cMessage* message)
{
  CurrentApplicationGuard guard{this};
  // TODO
}

int Application::decodeTransaction(uint16_t headerLength, const uint8_t* headerBuffer, uint32_t readlength,
                                   uint8_t* readBuffer)
{
  constexpr uint8_t operationMask{0x80};
  constexpr uint8_t registerFileMask{0x7F};

  const RegisterFile registerFile{static_cast<uint8_t>(headerBuffer[0] & registerFileMask)};
  const Operation operation{headerBuffer[0] & operationMask ? Operation::WRITE : Operation::READ};

  switch (registerFile) {
    case DEV_ID_ID:
      return handleReadDevId(readlength, readBuffer);
    default:
      throw cRuntimeError{"DecaWeave register file 0x%X is not supported", registerFile};
  }
}

int Application::handleReadDevId(uint32_t readlength, uint8_t* readBuffer)
{
  const uint32_t deviceId{0xDECA0130};
  memcpy(readBuffer, &deviceId, sizeof(deviceId));
  return DWT_SUCCESS;
}

Application* ApplicationSingleton::operator->()
{
  if (!application) {
    throw cRuntimeError{"Trying to use null smile::decaweave::Application instance"};
  }

  return application;
}

ApplicationSingleton& ApplicationSingleton::getInstance()
{
  return instance;
}

CurrentApplicationGuard::CurrentApplicationGuard(Application* currentApplication)
{
  assert(currentApplication);
  ApplicationSingleton::instance.application = currentApplication;
}

CurrentApplicationGuard::~CurrentApplicationGuard()
{
  ApplicationSingleton::instance.application = nullptr;
}

}  // namespace decaweave
}  // namespace smile

decaIrqStatus_t decamutexon()
{
  return 0;
}

void decamutexoff(decaIrqStatus_t s)
{
  return;
}

void deca_sleep(unsigned int time_ms)
{
  assert(true);  // TODO Implement me if needed!
  return;
}

int writetospi(uint16 headerLength, const uint8* headerBuffer, uint32 bodylength, const uint8* bodyBuffer)
{
  // TODO
  return -1;
}

int readfromspi(uint16 headerLength, const uint8* headerBuffer, uint32 readlength, uint8* readBuffer)
{
  auto& instance = smile::decaweave::ApplicationSingleton::getInstance();
  return instance->decodeTransaction(headerLength, headerBuffer, readlength, readBuffer);
}

#endif  // WITH_DECAWEAVE
