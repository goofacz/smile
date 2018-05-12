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

package smile;

//
// This module implements simple imperfect clock model. It is based on [1].
//
// [1] Y. Liu and C. Yang, "OMNeT++ based modeling and simulation of the IEEE 1588 PTP clock," 
// 2011 International Conference on Electrical and Control Engineering, Yichang, 2011, 
// pp. 4602-4605.
//
simple LiuYangClock extends Clock
{
    parameters:
        @class(smile::LiuYangClock);
        // Specify absolute range of drift for clock. SMILe will randomly select drift's sign and value.
        // Default range is +/- <10, 20> PPM
        double constant_drift_range = default(uniform(10e-6, 20e-6));

        // Do not modify this is internal formulas below
        double __constant_drift = select(intuniform(0, 1), -constant_drift_range, constant_drift_range);
}
