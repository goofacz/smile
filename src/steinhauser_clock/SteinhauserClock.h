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

#pragma once

#include <omnetpp.h>
#include "Clock.h"

namespace smile {
namespace steinhauser_clock {

class SteinhauserClock;
class StorageWindow;

/// \brief Implementation of a hardware (real, non perfect) clock.
///
/// This class implements a hardware clock according to the
/// paper "Accurate Clock Models for Simulating Wireless Sensor
/// Networks" by Federico Ferrari, Andreas Meier and Lothar Thiele.

class SteinhauserClock : public Clock
{
 public:
  /// Class to hold all properties of a simulated hardware clock.
  class Properties
  {
    omnetpp::simtime_t _tint;

    /// length of update interval (in # of tints)
    size_t _u{0};

    /// storage window size (in # of tints)
    size_t _s{0};

   public:
    Properties() = default;

    /// \returns	The time between two hold points of the
    ///		appproximated time function.
    const omnetpp::simtime_t& tint() const
    {
      return _tint;
    }

    /// \returns	The length of the update interval
    ///		(measured in number of times of tint).
    size_t u() const
    {
      return _u;
    }

    /// \returns	The size of the used storage window
    ///		(measured in number of times of tint).
    size_t s() const
    {
      return _s;
    }

    /// \returns	The time between two updates of the storage window.
    omnetpp::simtime_t updateInterval() const
    {
      return _tint * _u;
    }

    /// Sets the values that are held in the object.
    ///
    /// s is set to twice the value of u.
    ///
    /// \param tint	The new value for the time between two hold points.
    /// \param u	The new value for the length of the update interval.
    void set(const omnetpp::simtime_t& tint, size_t u);
  };

 private:
  /// The properties of this clock.
  Properties properties;

  StorageWindow* storageWindow{nullptr};

  /// Message to schedule storage window updates.
  omnetpp::cMessage* selfMsg{nullptr};

  /// Schedules the next update of the storage window.
  ///
  /// \param msg	The message used as a self message.
  void nextUpdate(omnetpp::cMessage* msg);

  /// Cleans up dynamically allocated resources
  /// between different simulation runs.
  void cleanup();

  /// Updates the text shown to the user in the GUI.
  void updateDisplay();

  /// Initializes the module.
  void initialize(int stage) override;

  /// Handles the given message.
  void handleMessage(omnetpp::cMessage* msg) override;

  /// Writes out statistics.
  void finish() override;

 public:
  /// Initializes the hardware clock.
  SteinhauserClock() = default;

  /// Cleans up the used resources of the hardware clock.
  ~SteinhauserClock();

  omnetpp::SimTime getClockTimestamp() override;

  OptionalSimTime convertToSimulationTimestamp(const omnetpp::SimTime& timestamp) override;
};

}  // namespace steinhauser_clock
}  // namespace smile
