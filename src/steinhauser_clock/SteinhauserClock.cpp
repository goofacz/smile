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

/*
    Clock Synchronization Simulation in OMNet++
    Copyright (C) 2012 Jens Steinhauser

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "SteinhauserClock.h"
#include <inet/common/INETDefs.h>
#include <exception>
#include "DriftSource.h"
#include "StorageWindow.h"

using namespace omnetpp;

namespace smile {
namespace steinhauser_clock {

Define_Module(SteinhauserClock);

void SteinhauserClock::Properties::set(const simtime_t& tint, size_t u)
{
  // minimum values
  simtime_t tint_min = SimTime::parse("1ms");
  size_t u_min = 10;

  if (tint < tint_min) {
    // EV << "warning: 'interval' value too small\n";
    _tint = tint_min;
  }
  else {
    _tint = tint;
  }

  if (u < u_min) {
    // EV << "warning: 'update' value too small\n";
    _u = u_min;
  }
  else {
    _u = u;
  }
  _s = 2 * _u;
}

SteinhauserClock::~SteinhauserClock()
{
  cancelAndDelete(selfMsg);

  cleanup();
}

void SteinhauserClock::nextUpdate(cMessage* msg)
{
  scheduleAt(simTime() + properties.updateInterval(), msg);
}

void SteinhauserClock::cleanup()
{
  if (storageWindow) {
    delete storageWindow;
    storageWindow = NULL;
  }

  // NOTE: selfMsg isn't deleted
}

void SteinhauserClock::initialize(int stage)
{
  Clock::initialize(stage);

  if (stage == inet::INITSTAGE_LOCAL) {
    // if needed, clean up stuff from the last run
    cleanup();

    properties.set(par("interval"), par("update"));

    EV << "update interval: " << properties.updateInterval() << "s\n";

    DriftSource* d = NULL;

    if (hasPar("drift_distribution")) {
      if (hasPar("max_drift_variation")) {
        d = new BoundedDriftVariation(par("drift_distribution"), par("max_drift_variation"), par("start_value"),
                                      properties.tint());
      }
      else {
        d = new BoundedDrift(par("drift_distribution"));
      }
    }
    else {
      d = new ConstantDrift(par("constant_drift"));
    }

    storageWindow = new StorageWindow(properties, d);
    updateDisplay();

    cMessage* msg = new cMessage("storage window update");
    nextUpdate(msg);
  }
}

void SteinhauserClock::handleMessage(cMessage* msg)
{
  if (msg->isSelfMessage()) {
    // the only self message is to update the storage window

    storageWindow->update();
    updateDisplay();

    emit(windowUpdateSignal, getClockTimestamp());

    nextUpdate(msg);
  }
}

void SteinhauserClock::finish()
{
  if (storageWindow) {
    storageWindow->finish();
  }
}

void SteinhauserClock::updateDisplay()
{
  if (!getEnvir()->isGUI() || getEnvir()->isLoggingEnabled()) {
    // skip this if GUI is not running or in express mode
    return;
  }

  simtime_t real = simTime();
  const simtime_t& hard = storageWindow->hardwareTimeBegin();

  simtime_t diff = hard - real;
  double d = fabs(diff.dbl());

  const char sign = diff < 0 ? '-' : '+';
  const char* unit;
  int val;

  if (d > 1) {
    val = d;
    unit = "s";
  }
  else if (d > 1e-3) {
    val = d * 1e3;
    unit = "ms";
  }
  else if (d > 1e-6) {
    val = d * 1e6;
    unit = "us";
  }
  else if (d > 1e-9) {
    val = d * 1e9;
    unit = "ns";
  }
  else if (d > 1e-12) {
    val = d * 1e12;
    unit = "ps";
  }
  else {
    getDisplayString().setTagArg("t", 0, "delta t: ca. 0s");
    return;
  }

  char buf[64];
  snprintf(buf, sizeof(buf), "delta t: ca. %c%i%s", sign, val, unit);

  getDisplayString().setTagArg("t", 0, buf);
}

omnetpp::SimTime SteinhauserClock::getClockTimestamp()
{
  const simtime_t now = simTime();
  const int k = storageWindow->indexOf(now);
  const StorageWindow::HoldPoint& hp = storageWindow->at(k);
  const simtime_t t = now - hp.realTime;
  return hp.hardwareTime + t * (1 + hp.drift);
}

Clock::OptionalSimTime SteinhauserClock::convertToSimulationTimestamp(const omnetpp::SimTime& timestamp)
{
  if (timestamp < storageWindow->at(0).hardwareTime || timestamp > storageWindow->hardwareTimeEnd()) {
    // outside of storage window, can't translate timestamp
    return {};
  }

  // the current interval is the lower limit for the
  // interval the hardware time is in
  size_t k = storageWindow->indexOf(simTime());

  // find the correct interval
  while (k != properties.s() - 1 && storageWindow->at(k + 1).hardwareTime < timestamp) {
    k++;
  }

  const StorageWindow::HoldPoint& hp = storageWindow->at(k);
  return hp.realTime + (timestamp - hp.hardwareTime) / (1 + hp.drift);
}

}  // namespace steinhauser_clock
}  // namespace smile
