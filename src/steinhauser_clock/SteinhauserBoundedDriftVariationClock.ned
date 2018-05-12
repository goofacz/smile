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

//
//  Clock Synchronization Simulation in OMNet++
//  Copyright (C) 2012 Jens Steinhauser
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

package smile.steinhauser_clock;

//
// Module implements clock model described in 
// F. Ferrari, A. Meier, L. Thiele, "Accurate Clock Models for Simulating
// Wireless Sensor Networks", Proceedings of the 3rd International ICST 
// Conference on Simulation Tools and Techniques, 2011, Article No. 21 
//
// Original implementation available at https://github.com/JenSte/omnet-ptp
//

simple SteinhauserBoundedDriftVariationClock extends SteinhauserClock {
    parameters:
        double drift_distribution_range = default(uniform(10e-6, 20e-6));
        double start_value = default(0.0);
        double max_drift_variation = default(10e-8);

        // Do not modify this is internal formulas below
        volatile double __drift_distribution = select(intuniform(0, 1), -drift_distribution_range, drift_distribution_range);
}
