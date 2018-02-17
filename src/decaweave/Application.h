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

#include <functional>
#include "../Application.h"
#include "deca_device_api.h"

namespace smile {
namespace decaweave {

class Application : public smile::Application
{
  friend class CurrentApplicationGuard;

private:
  using DecalibCallback = std::function<void(const dwt_cb_data_t*)>;

 public:
  Application() = default;
  Application(const Application& source) = delete;
  Application(Application&& source) = delete;
  ~Application() = default;

  Application& operator=(const Application& source) = delete;
  Application& operator=(Application&& source) = delete;

  static int dwt_setlocaldataptr(unsigned int index);
  static uint32 dwt_getpartid();
  static uint32 dwt_getlotid();
  static uint32 dwt_readdevid();
  static uint8 dwt_otprevision();
  static void dwt_setfinegraintxseq(int enable);
  static void dwt_setlnapamode(int lna, int pa);
  static void dwt_setgpiodirection(uint32 gpioNum, uint32 direction);
  static void dwt_setgpiovalue(uint32 gpioNum, uint32 value);
  static int dwt_initialise(uint16 config);
  static void dwt_configure(dwt_config_t* config);
  static void dwt_configuretxrf(dwt_txconfig_t* config);
  static void dwt_setrxantennadelay(uint16 antennaDly);
  static void dwt_settxantennadelay(uint16 antennaDly);
  static void dwt_setsmarttxpower(int enable);
  static int dwt_writetxdata(uint16 txFrameLength, uint8* txFrameBytes, uint16 txBufferOffset);
  static void dwt_writetxfctrl(uint16 txFrameLength, uint16 txBufferOffset, int ranging);
  static int dwt_starttx(uint8 mode);
  static void dwt_setdelayedtrxtime(uint32 starttime);
  static void dwt_readtxtimestamp(uint8* timestamp);
  static uint32 dwt_readtxtimestamphi32();
  static uint32 dwt_readtxtimestamplo32();
  static void dwt_readrxtimestamp(uint8* timestamp);
  static uint32 dwt_readrxtimestamphi32();
  static uint32 dwt_readrxtimestamplo32();
  static uint32 dwt_readsystimestamphi32();
  static void dwt_readsystime(uint8* timestamp);
  static void dwt_forcetrxoff();
  static void dwt_syncrxbufptrs();
  static int dwt_rxenable(int mode);
  static void dwt_setsniffmode(int enable, uint8 timeOn, uint8 timeOff);
  static void dwt_setlowpowerlistening(int enable);
  static void dwt_setsnoozetime(uint8 snooze_time);
  static void dwt_setdblrxbuffmode(int enable);
  static void dwt_setrxtimeout(uint16 time);
  static void dwt_setpreambledetecttimeout(uint16 timeout);
  static uint16 dwt_calibratesleepcnt();
  static void dwt_configuresleepcnt(uint16 sleepcnt);
  static void dwt_configuresleep(uint16 mode, uint8 wake);
  static void dwt_entersleep();
  static void dwt_entersleepaftertx(int enable);
  static int dwt_spicswakeup(uint8* buff, uint16 length);
  static void dwt_setcallbacks(dwt_cb_t cbTxDone, dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr);
  static uint8 dwt_checkirq();
  static void dwt_isr();
  static void dwt_lowpowerlistenisr();
  static void dwt_setinterrupt(uint32 bitmask, uint8 enable);
  static void dwt_setpanid(uint16 panID);
  static void dwt_setaddress16(uint16 shortAddress);
  static void dwt_seteui(uint8* eui64);
  static void dwt_geteui(uint8* eui64);
  static void dwt_otpread(uint32 address, uint32* array, uint8 length);
  static void dwt_enableframefilter(uint16 bitmask);
  static void dwt_enableautoack(uint8 responseDelayTime);
  static void dwt_setrxaftertxdelay(uint32 rxDelayTime);
  static void dwt_rxreset();
  static void dwt_softreset();
  static void dwt_readrxdata(uint8* buffer, uint16 length, uint16 rxBufferOffset);
  static void dwt_readaccdata(uint8* buffer, uint16 length, uint16 rxBufferOffset);
  static int32 dwt_readcarrierintegrator();
  static void dwt_readdiagnostics(dwt_rxdiag_t* diagnostics);
  static void dwt_loadopsettabfromotp(uint8 ops_sel);
  static void dwt_configeventcounters(int enable);
  static void dwt_readeventcounters(dwt_deviceentcnts_t* counters);
  static int dwt_otpwriteandverify(uint32 value, uint16 address);
  static void dwt_setleds(uint8 mode);
  static void dwt_setxtaltrim(uint8 value);
  static uint8 dwt_getinitxtaltrim();
  static void dwt_configcwmode(uint8 chan);
  static void dwt_configcontinuousframemode(uint32 framerepetitionrate);
  static uint16 dwt_readtempvbat(uint8 fastSPI);
  static uint8 dwt_readwakeuptemp();
  static uint8 dwt_readwakeupvbat();
  static uint32 dwt_calcbandwidthtempadj(uint16 target_count);
  static uint32 dwt_calcpowertempadj(uint8 channel, uint32 ref_powerreg, double current_temperature,
                              double reference_temperature);
  static uint16 dwt_calcpgcount(uint8 pgdly);
  static void dwt_writetodevice(uint16 recordNumber, uint16 index, uint32 length, const uint8* buffer);
  static void dwt_readfromdevice(uint16 recordNumber, uint16 index, uint32 length, uint8* buffer);
  static uint32 dwt_read32bitoffsetreg(int regFileID, int regOffset);
  static void dwt_write32bitoffsetreg(int regFileID, int regOffset, uint32 regval);
  static uint16 dwt_read16bitoffsetreg(int regFileID, int regOffset);
  static void dwt_write16bitoffsetreg(int regFileID, int regOffset, uint16 regval);
  static uint8 dwt_read8bitoffsetreg(int regFileID, int regOffset);
  static void dwt_write8bitoffsetreg(int regFileID, int regOffset, uint8 regval);
  static void deca_sleep(unsigned int time_ms);

 protected:
  void initialize(int stage) override;

  virtual void main();

 private:
  void handleIncommingMessage(cMessage* message) override final;

  DecalibCallback decalibTxDoneCallback;
  DecalibCallback decalibRxOkCallback;
  DecalibCallback decalibRxToCallback;
  DecalibCallback decalibRxErrCallback;
};

}  // namespace decaweave
}  // namespace smile

#endif  // WITH_DECAWEAVE
