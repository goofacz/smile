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

#include <cassert>

#include "DecaWeaveNicDriver.h"

#ifdef WITH_DECAWEAVE

#include "../utilities.h"
#include "deca_device_api.h"

namespace smile {

class NicModuleSingleton final
{
 public:
  friend class NicModuleSingletonGuard;

  NicModuleSingleton(const NicModuleSingleton& source) = delete;
  NicModuleSingleton(NicModuleSingleton&& source) = delete;
  ~NicModuleSingleton() = default;

  NicModuleSingleton& operator=(const NicModuleSingleton& source) = delete;
  NicModuleSingleton& operator=(NicModuleSingleton&& source) = delete;

  DecaWeaveNicDriver* operator->();

 private:
  NicModuleSingleton() = default;

  static NicModuleSingleton instance;

  DecaWeaveNicDriver* nic{nullptr};
};

class NicModuleSingletonGuard final
{
 public:
  explicit NicModuleSingletonGuard(DecaWeaveNicDriver* currentNicModule);
  NicModuleSingletonGuard(const NicModuleSingletonGuard& source) = delete;
  NicModuleSingletonGuard(NicModuleSingletonGuard&& source) = delete;
  ~NicModuleSingletonGuard();

  NicModuleSingletonGuard& operator=(const NicModuleSingletonGuard& source) = delete;
  NicModuleSingletonGuard& operator=(NicModuleSingletonGuard&& source) = delete;
};

NicModuleSingleton NicModuleSingleton::instance;

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

Define_Module(DecaWeaveNicDriver);

inet::MACAddress DecaWeaveNicDriver::getMacAddress() const
{
  return inet::MACAddress{mac->par("address").stringValue()};
}

void DecaWeaveNicDriver::initialize(int stage)
{
  ClockDecorator<cSimpleModule>::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    const auto nicModulePath = par("nicModuleRelativePath").stringValue();
    auto nic = getModuleByPath(nicModulePath);
    if (!nic) {
      throw cRuntimeError{"Failed to find \"%s\" module", nicModulePath};
    }

    const auto macModulePath = ".mac";
    mac = nic->getModuleByPath(macModulePath);
    if (!mac) {
      throw cRuntimeError{"Failed to find \"%s\" module relative to \"nic\" module", macModulePath};
    }
  }
}

void DecaWeaveNicDriver::handleIncommingMessage(omnetpp::cMessage* newMessage)
{
  NicModuleSingletonGuard{this};
  // TODO
}

void DecaWeaveNicDriver::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, long value,
                                       omnetpp::cObject* details)
{
  // TODO
}

DecaWeaveNicDriver* NicModuleSingleton::operator->()
{
  if (!nic) {
    throw cRuntimeError{"Trying to use null NicModuleSingleton instance"};
  }

  return nic;
}

NicModuleSingletonGuard::NicModuleSingletonGuard(DecaWeaveNicDriver* currentNicModule)
{
  NicModuleSingleton::instance.nic = currentNicModule;
}

NicModuleSingletonGuard::~NicModuleSingletonGuard()
{
  NicModuleSingleton::instance.nic = nullptr;
}

}  // namespace smile

#endif  // WITH_DECAWEAVE
