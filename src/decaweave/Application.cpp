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

int Application::dwt_setlocaldataptr(unsigned int index)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_getpartid()
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_getlotid()
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_readdevid()
{
  return 0xDECA0130;
}

uint8 Application::dwt_otprevision()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setfinegraintxseq(int enable)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setlnapamode(int lna, int pa)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setgpiodirection(uint32 gpioNum, uint32 direction)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setgpiovalue(uint32 gpioNum, uint32 value)
{
  throw cRuntimeError{"decalib function not implemented"};
}

int Application::dwt_initialise(uint16 config)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_configure(dwt_config_t* config)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_configuretxrf(dwt_txconfig_t* config)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setrxantennadelay(uint16 antennaDly)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_settxantennadelay(uint16 antennaDly)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setsmarttxpower(int enable)
{
  throw cRuntimeError{"decalib function not implemented"};
}

int Application::dwt_writetxdata(uint16 txFrameLength, uint8* txFrameBytes, uint16 txBufferOffset)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_writetxfctrl(uint16 txFrameLength, uint16 txBufferOffset, int ranging)
{
  throw cRuntimeError{"decalib function not implemented"};
}

int Application::dwt_starttx(uint8 mode)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setdelayedtrxtime(uint32 starttime)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_readtxtimestamp(uint8* timestamp)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_readtxtimestamphi32()
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_readtxtimestamplo32()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_readrxtimestamp(uint8* timestamp)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_readrxtimestamphi32()
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_readrxtimestamplo32()
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_readsystimestamphi32()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_readsystime(uint8* timestamp)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_forcetrxoff()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_syncrxbufptrs()
{
  throw cRuntimeError{"decalib function not implemented"};
}

int Application::dwt_rxenable(int mode)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setsniffmode(int enable, uint8 timeOn, uint8 timeOff)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setlowpowerlistening(int enable)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setsnoozetime(uint8 snooze_time)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setdblrxbuffmode(int enable)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setrxtimeout(uint16 time)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setpreambledetecttimeout(uint16 timeout)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint16 Application::dwt_calibratesleepcnt()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_configuresleepcnt(uint16 sleepcnt)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_configuresleep(uint16 mode, uint8 wake)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_entersleep()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_entersleepaftertx(int enable)
{
  throw cRuntimeError{"decalib function not implemented"};
}

int Application::dwt_spicswakeup(uint8* buff, uint16 length)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setcallbacks(dwt_cb_t cbTxDone, dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint8 Application::dwt_checkirq()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_isr()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_lowpowerlistenisr()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setinterrupt(uint32 bitmask, uint8 enable)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setpanid(uint16 panID)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setaddress16(uint16 shortAddress)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_seteui(uint8* eui64)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_geteui(uint8* eui64)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_otpread(uint32 address, uint32* array, uint8 length)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_enableframefilter(uint16 bitmask)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_enableautoack(uint8 responseDelayTime)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setrxaftertxdelay(uint32 rxDelayTime)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_rxreset()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_softreset()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_readrxdata(uint8* buffer, uint16 length, uint16 rxBufferOffset)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_readaccdata(uint8* buffer, uint16 length, uint16 rxBufferOffset)
{
  throw cRuntimeError{"decalib function not implemented"};
}

int32 Application::dwt_readcarrierintegrator()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_readdiagnostics(dwt_rxdiag_t* diagnostics)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_loadopsettabfromotp(uint8 ops_sel)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_configeventcounters(int enable)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_readeventcounters(dwt_deviceentcnts_t* counters)
{
  throw cRuntimeError{"decalib function not implemented"};
}

int Application::dwt_otpwriteandverify(uint32 value, uint16 address)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setleds(uint8 mode)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_setxtaltrim(uint8 value)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint8 Application::dwt_getinitxtaltrim()
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_configcwmode(uint8 chan)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_configcontinuousframemode(uint32 framerepetitionrate)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint16 Application::dwt_readtempvbat(uint8 fastSPI)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint8 Application::dwt_readwakeuptemp()
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint8 Application::dwt_readwakeupvbat()
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_calcbandwidthtempadj(uint16 target_count)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_calcpowertempadj(uint8 channel, uint32 ref_powerreg, double current_temperature,
                            double reference_temperature)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint16 Application::dwt_calcpgcount(uint8 pgdly)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_writetodevice(uint16 recordNumber, uint16 index, uint32 length, const uint8* buffer)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_readfromdevice(uint16 recordNumber, uint16 index, uint32 length, uint8* buffer)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint32 Application::dwt_read32bitoffsetreg(int regFileID, int regOffset)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_write32bitoffsetreg(int regFileID, int regOffset, uint32 regval)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint16 Application::dwt_read16bitoffsetreg(int regFileID, int regOffset)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_write16bitoffsetreg(int regFileID, int regOffset, uint16 regval)
{
  throw cRuntimeError{"decalib function not implemented"};
}

uint8 Application::dwt_read8bitoffsetreg(int regFileID, int regOffset)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void Application::dwt_write8bitoffsetreg(int regFileID, int regOffset, uint8 regval)
{
  throw cRuntimeError{"decalib function not implemented"};
}

void deca_sleep(unsigned int time_ms)
{
  throw cRuntimeError{"decalib function not implemented"};
}

}  // namespace decaweave
}  // namespace smile

#endif  // WITH_DECAWEAVE
