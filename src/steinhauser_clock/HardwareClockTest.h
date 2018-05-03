#pragma once

#include <omnetpp.h>
#include <vector>

namespace smile {
namespace steinhauser_clock {

class HardwareClockTest : public omnetpp::cSimpleModule
{
 private:
  using Timestamps = std::vector<omnetpp::SimTime>;

  void initialize() override;
  void handleMessage(omnetpp::cMessage *msg) override;
  void finish() override;
  std::vector<Timestamps> timestamps;
};

}  // namespace steinhauser_clock
}  // namespace smile
