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

using namespace omnetpp;

namespace smile::clocks{

Define_Module(SteinhauserClock);

double SteinhauserClock::DriftSource::nextValue()
{
  const double driftLimit = -0.999999;
  const double n = next();

  // limit drift to values > -1, so the time can't go back
  if (n < driftLimit) {
    return driftLimit;
  }

  return n;
}

SteinhauserClock::ConstantDrift::ConstantDrift(double drift) : drift(drift) {}

double SteinhauserClock::ConstantDrift::next()
{
  return drift;
}

SteinhauserClock::BoundedDrift::BoundedDrift(const cPar& distribution) : distribution(distribution) {}

double SteinhauserClock::BoundedDrift::next()
{
  return distribution.doubleValue();
}

SteinhauserClock::BoundedDriftVariation::BoundedDriftVariation(const cPar& distribution, double max_drift_variation,
                                                               double start_value, const simtime_t& tint) :
    BoundedDrift(distribution),
    max_drift_change(tint.dbl() * max_drift_variation),
    last_drift(start_value)
{}

double SteinhauserClock::BoundedDriftVariation::next()
{
  double drift = BoundedDrift::next();

  double diff = drift - last_drift;

  // limit the drift
  if (diff > max_drift_change) {
    drift = last_drift + max_drift_change;
  }
  else if (diff < -max_drift_change) {
    drift = last_drift - max_drift_change;
  }

  last_drift = drift;
  return drift;
}

SteinhauserClock::StorageWindow::StorageWindow(const SteinhauserClock::Properties& properties, DriftSource* source) :
    properties(properties),
    source(source)
{
  data.resize(properties.s());

  std::vector<HoldPoint>::iterator it = data.begin();

  driftVector.setName("drift");
  timeVector.setName("hardware_time");
  deviationVector.setName("time_deviation");
  driftHistogram.setName("drift_values");

  timeVector.setUnit("s");
  deviationVector.setUnit("s");

  simtime_t now = simTime();

  it->realTime = now;
  it->hardwareTime = now;
  it->drift = source->nextValue();

  recordVectors(now, now, it->drift);

  fillRange(it + 1, data.end());
}

SteinhauserClock::StorageWindow::~StorageWindow()
{
  delete source;
}

void SteinhauserClock::StorageWindow::finish()
{
  driftHistogram.recordAs("drift_distribution");
}

void SteinhauserClock::StorageWindow::update()
{
  data = std::vector<HoldPoint>(data.begin() + properties.u(), data.end());
  data.resize(properties.s());

  fillRange(data.begin() + (properties.s() - properties.u()), data.end());
}

void SteinhauserClock::StorageWindow::fillRange(std::vector<HoldPoint>::iterator first,
                                                std::vector<HoldPoint>::iterator last)
{
  while (first != last) {
    std::vector<HoldPoint>::iterator pre = first - 1;

    first->realTime = pre->realTime + properties.tint();
    first->hardwareTime = pre->hardwareTime + properties.tint() * (1 + pre->drift);
    first->drift = source->nextValue();
    recordVectors(first->realTime, first->hardwareTime, first->drift);

    first++;
  }

  _hardwareTimeEnd = data[data.size() - 1].hardwareTime + properties.tint() * (1 + data[data.size() - 1].drift);
}

void SteinhauserClock::StorageWindow::recordVectors(const simtime_t& realTime, const simtime_t& hardwareTime,
                                                    double drift)
{
  driftHistogram.collect(drift);

  driftVector.recordWithTimestamp(realTime, drift);
  timeVector.recordWithTimestamp(realTime, hardwareTime);
  deviationVector.recordWithTimestamp(realTime, hardwareTime - realTime);
}

const SteinhauserClock::StorageWindow::HoldPoint& SteinhauserClock::StorageWindow::at(size_t idx) const
{
  if (idx > data.size() - 1) {
    throw std::logic_error("StorageWindow::HoldPoint: index out of bounds");
  }

  return data[idx];
}

size_t SteinhauserClock::StorageWindow::indexOf(const simtime_t& t) const
{
  return (t - data[0].realTime) / properties.tint();
}

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

    if (hasPar("__drift_distribution")) {
      if (hasPar("max_drift_variation")) {
        d = new BoundedDriftVariation(par("__drift_distribution"), par("max_drift_variation"), par("start_value"),
                                      properties.tint());
      }
      else {
        d = new BoundedDrift(par("__drift_distribution"));
      }
    }
    else {
      d = new ConstantDrift(par("__constant_drift"));
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
