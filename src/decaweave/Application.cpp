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
  ApplicationSingleton(const ApplicationSingleton& source) = delete;
  ApplicationSingleton(ApplicationSingleton&& source) = delete;
  ~ApplicationSingleton() = default;

  ApplicationSingleton& operator=(const ApplicationSingleton& source) = delete;
  ApplicationSingleton& operator=(ApplicationSingleton&& source) = delete;

  Application* operator->();
  void setApplication(Application* newApplication);

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

Application::Application() : smile::Application(), decaLibIndex{generateDecaLibIndex()}
{
  resetRegisterFiles();
}

void Application::initialize(int stage)
{
  smile::Application::initialize(stage);
}

unsigned int Application::generateDecaLibIndex()
{
  static unsigned int index{0};
  return index++;
}

void Application::handleIncommingMessage(cMessage* message)
{
  CurrentApplicationGuard guard{this};
  // TODO
}

int Application::decodeTransaction(uint16_t headerLength, const uint8_t* headerBuffer, uint32_t dataLength,
                                   uint8_t* readBuffer, const uint8_t* writeBuffer)
{
  constexpr uint8_t operationMask{0x80};
  constexpr uint8_t registerFileMask{0x7F};

  RegisterOffset offset{0};
  switch (headerLength) {
    case 1:
      break;
    case 2:
      offset = (headerBuffer[1] & 0x7F);
      break;
    case 3:
      offset = (headerBuffer[2] << 7);
      offset |= (headerBuffer[1] & 0x7F);
      break;
    default:
      throw cRuntimeError{"DecaWeave transaction header size (%d bytes) is not supported", headerLength};
  }

  const RegisterFile registerFile{static_cast<RegisterFile>(headerBuffer[0] & registerFileMask)};
  const Operation operation{headerBuffer[0] & operationMask ? Operation::WRITE : Operation::READ};
  switch (operation) {
    case Operation::WRITE:
      assert(writeBuffer);
      return handleWriteTransaction({registerFile, offset}, dataLength, writeBuffer);
    case Operation::READ:
      assert(readBuffer);
      return handleReadTransaction({registerFile, offset}, dataLength, readBuffer);
  }
}

int Application::handleReadTransaction(const FullRegisterFile& fullRegisterFile, uint32_t readLength,
                                       uint8_t* readBuffer)
{
  try {
    const auto& value = registerFiles.at(fullRegisterFile.first);
    std::memcpy(readBuffer, value.data() + fullRegisterFile.second, readLength);
    return DWT_SUCCESS;
  }
  catch (const std::out_of_range&) {
    throw cRuntimeError{"Reading DecaWeave register file 0x%X:%x is not supported", fullRegisterFile.first,
                        fullRegisterFile.second};
  }
}

int Application::handleWriteTransaction(const FullRegisterFile& fullRegisterFile, uint32_t writeLength,
                                        const uint8_t* writeBuffer)
{
  try {
    auto& value = registerFiles.at(fullRegisterFile.first);
    std::memcpy(value.data() + fullRegisterFile.second, writeBuffer, writeLength);
    return DWT_SUCCESS;
  }
  catch (const std::out_of_range&) {
    throw cRuntimeError{"Writing DecaWeave register file 0x%X:%x is not supported", fullRegisterFile.first,
                        fullRegisterFile.second};
  }
}

unsigned int Application::getDecaLibIndex() const
{
  return decaLibIndex;
}

void Application::resetRegisterFiles()
{
  registerFiles.clear();
  registerFiles[DEV_ID_ID, 0] = {0xDE, 0xCA, 0x01, 0x30};
  registerFiles[PMSC_ID] = {0b11110000, 0b00110000, 0b00000010, 0b00000000, /* CTRL0 */
                            0b10000001, 0b00000010, 0b00000111, 0b00111000 /* CTRL1 */};
}

Application* ApplicationSingleton::operator->()
{
  if (!application) {
    throw cRuntimeError{"Trying to use null smile::decaweave::Application instance"};
  }

  return application;
}
void ApplicationSingleton::setApplication(Application* newApplication)
{
  application = newApplication;
}

ApplicationSingleton& ApplicationSingleton::getInstance()
{
  return instance;
}

CurrentApplicationGuard::CurrentApplicationGuard(Application* currentApplication)
{
  assert(currentApplication);
  const auto index = currentApplication->getDecaLibIndex();

  const auto result = dwt_setlocaldataptr(index);
  if (result == DWT_ERROR) {
    throw cRuntimeError{"Failed to call dwt_setlocaldataptr() with index=%d (DWT_NUM_DW_DEV=%d)", index,
                        DWT_NUM_DW_DEV};
  }

  auto& instance = ApplicationSingleton::getInstance();
  instance.setApplication(currentApplication);
}

CurrentApplicationGuard::~CurrentApplicationGuard()
{
  auto& instance = ApplicationSingleton::getInstance();
  instance.setApplication(nullptr);
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
  auto& instance = smile::decaweave::ApplicationSingleton::getInstance();
  return instance->decodeTransaction(headerLength, headerBuffer, bodylength, nullptr, bodyBuffer);
}

int readfromspi(uint16 headerLength, const uint8* headerBuffer, uint32 readlength, uint8* readBuffer)
{
  auto& instance = smile::decaweave::ApplicationSingleton::getInstance();
  return instance->decodeTransaction(headerLength, headerBuffer, readlength, readBuffer, nullptr);
}

#endif  // WITH_DECAWEAVE
