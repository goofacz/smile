# Simulator for Methods of Indoor Localization

SMILe allows analyzing and evaluating arbitrary indoor localization methods utilizing time measurements. Project is a framework that [OMNeT++](https://omnetpp.org/) and it's heavily utilizes other OMNeT++ framework - [INET](https://github.com/inet-framework/inet).

## Installation

1. Make sure you have C++ compiler supporting C++14 installed on your platform.
2. Install OMNeT++ 5.2 (or newer). After extracting tarball remember to edit file `configure.user`. Open it and uncomment variable `CXXFLAGS` and assign it with value `-std=c++14`.
3. Install INET 3.6.2 (or newer).
4. Install SMILe. **Important note:** Remember to checkout SMILe next to INET.

## Authors

Tomasz Jankowski, MSc

Maciej Nikodem, PhD


SMILe is developed at [Wrocław University of Science and Technology](http://pwr.edu.pl/).
