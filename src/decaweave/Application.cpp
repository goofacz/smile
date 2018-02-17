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

void Application::initialize(int stage)
{
  smile::Application::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    main();
  }
}

void Application::main()
{
    throw cRuntimeError{"Application::main() have to be overridden!"};
}

void Application::dwt_setcallbacks(dwt_cb_t cbTxDone, dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr)
{
  auto& application = ApplicationSingleton::getInstance();
  application->decalibTxDoneCallback = cbTxDone;
  application->decalibRxOkCallback = cbRxOk;
  application->decalibRxToCallback = cbRxTo;
  application->decalibRxErrCallback = cbRxErr;
}

uint32 Application::dwt_readdevid()
{
  return 0xDECA0130;
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

#endif  // WITH_DECAWEAVE
