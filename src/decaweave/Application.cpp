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
#include "deca_device_api.h"

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

}  // namespace decaweave
}  // namespace smile

extern "C" {

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
  // TODO
  return -1;
}

}  // extern "C"

namespace smile {
namespace decaweave {

Define_Module(Application);

void Application::initialize(int stage)
{
  smile::Application::initialize(stage);
  // TODO
}

void Application::handleIncommingMessage(cMessage* message)
{
  CurrentApplicationGuard{this};
  // TODO
}

Application* ApplicationSingleton::operator->()
{
  if (!application) {
    throw cRuntimeError{"Trying to use null smile::decaweave::Application instance"};
  }

  return application;
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

#endif  // WITH_DECAWEAVE
