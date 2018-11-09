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

#pragma once

#include <omnetpp.h>
#include "Clock.h"

namespace smile::clocks {

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

    /// \returns    The time between two hold points of the
    ///     appproximated time function.
    const omnetpp::simtime_t& tint() const
    {
      return _tint;
    }

    /// \returns    The length of the update interval
    ///     (measured in number of times of tint).
    size_t u() const
    {
      return _u;
    }

    /// \returns    The size of the used storage window
    ///     (measured in number of times of tint).
    size_t s() const
    {
      return _s;
    }

    /// \returns    The time between two updates of the storage window.
    omnetpp::simtime_t updateInterval() const
    {
      return _tint * _u;
    }

    /// Sets the values that are held in the object.
    ///
    /// s is set to twice the value of u.
    ///
    /// \param tint The new value for the time between two hold points.
    /// \param u    The new value for the length of the update interval.
    void set(const omnetpp::simtime_t& tint, size_t u);
  };

 private:
  /// Abstract base class for drift value sources.
  class DriftSource
  {
   protected:
    virtual double next() = 0;

   public:
    virtual ~DriftSource() = default;

    /// \returns  The next drift value.
    double nextValue();
  };

  /// \brief A constant drift source.
  ///
  /// This source always returns the same constant drift.
  class ConstantDrift : public DriftSource
  {
   private:
    double drift;

   protected:
    double next();

   public:
    /// Initalizes the object.
    ///
    /// \param drift  The constant drift value.
    ConstantDrift(double drift);
  };

  /// \brief Source for drift values within certain bounds.
  class BoundedDrift : public DriftSource
  {
   private:
    const omnetpp::cPar& distribution;

   protected:
    double next();

   public:
    /// Initalizes the object.
    ///
    /// \param distribution   A OMNeT++ module parameter that evaluates to a distribution.
    BoundedDrift(const omnetpp::cPar& distribution);
  };

  /// \brief Source for drift values within bounds that are limited in their variation.
  class BoundedDriftVariation : public BoundedDrift
  {
   private:
    /// Maximum value that the drift can change between
    /// two values (between tint).
    double max_drift_change;

    double last_drift;

   protected:
    double next();

   public:
    /// Initalizes the object.
    ///
    /// After evaluation the 'distribution' parameter, the resulting
    /// value is limited to the product of 'max_drift_variation' and 'tint'.
    ///
    /// \param distribution   A OMNeT++ module parameter that evaluates to a distribution.
    /// \param max_drift_variation    The maximum value for \f$\frac{d}{dt}drift\f$.
    /// \param start_value    Value of the drift before the simulation starts (used to limit the
    ///           variation of the first drift value).
    /// \param tint   Simulation time between two drift values.
    BoundedDriftVariation(const omnetpp::cPar& distribution, double max_drift_variation, double start_value,
                          const omnetpp::simtime_t& tint);
  };

  /// Saves the data points for the continuous linear approximation of the drift function.
  class StorageWindow
  {
   public:
    /// Represents a hold point of the continuous linear approximation.
    struct HoldPoint
    {
      /// The simulation time at the point.
      omnetpp::simtime_t realTime;

      /// The hardware time at the point.
      omnetpp::simtime_t hardwareTime;

      /// The drift value between this and the next point.
      double drift;
    };

   private:
    /// Holds the data of the approximation.
    std::vector<HoldPoint> data;

    /// The properties of the clock this object belongs to.
    const SteinhauserClock::Properties& properties;

    /// The object that supplies the drift values to fill
    /// the data vector.
    DriftSource* source{nullptr};

    /// Vector to record the drift values.
    omnetpp::cOutVector driftVector;

    /// Vector to record the hardware timestamps.
    omnetpp::cOutVector timeVector;

    /// Vector to record the deviation between the hardware
    /// and simulation time.
    omnetpp::cOutVector deviationVector;

    // Collects statistics about the drift values.
    omnetpp::cDoubleHistogram driftHistogram;

    /// The hardware timestamp of the first point after the
    /// storage window.
    omnetpp::simtime_t _hardwareTimeEnd;

    /// Fills the range [first, last) with new timestamp/drift values.
    void fillRange(std::vector<HoldPoint>::iterator first, std::vector<HoldPoint>::iterator last);

    /// Records the given values to the vector files.
    ///
    /// \param realTime   The simulation timestamp, also used to calculate the time deviation.
    ///           Has to be increasing between two calls to the function.
    /// \param hardwareTime   The hardware timestamp at realTime.
    /// \param drift  The drift value at realTime.
    void recordVectors(const omnetpp::simtime_t& realTime, const omnetpp::simtime_t& hardwareTime, double drift);

   public:
    /// Initializes the storage window.
    ///
    /// \param properties Properties object of the simulated hardware clock, used
    ///           to determine things like the length of the storage window, etc.
    /// \param source Pointer to a source of drift values, the StorageWindow object
    ///           takes ownership of the object being passed.
    StorageWindow(const SteinhauserClock::Properties& properties, DriftSource* source);

    ~StorageWindow();

    /// Writes out statistics.
    void finish();

    /// Updates the storage window.
    ///
    /// The first u values in the storge window are discarded, the
    /// remaining ones are moved to the front and the empty space
    /// is filled up with new values.
    void update();

    /// Returns the time at the beginning of the storage window.
    const omnetpp::simtime_t& hardwareTimeBegin() const
    {
      return data[0].hardwareTime;
    }

    /// Returns the time at the end of the storage window.
    const omnetpp::simtime_t& hardwareTimeEnd() const
    {
      return _hardwareTimeEnd;
    }

    /// Returns the hold point at index idx.
    ///
    /// If the index is out of bounds, an std::logic_error exception is thrown.
    /// \param idx    The index of the requested hold point.
    /// \returns  The hold point at index idx.
    const StorageWindow::HoldPoint& at(size_t idx) const;

    /// Calculates the hold point index for a timestamp.
    ///
    /// \param t  A simulation timestamp.
    /// \returns  The index of the hold point in what the simulation time lies.
    size_t indexOf(const omnetpp::simtime_t& t) const;
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
