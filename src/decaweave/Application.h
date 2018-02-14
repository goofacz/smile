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

#include "../Application.h"

namespace smile {
namespace decaweave {

class Application : public smile::Application
{
 private:
  enum class Operation : uint8_t
  {
    READ = 0x0,
    WRITE = 0x1,
    READ_WRITE = 0x10
  };

  struct TransactionDescriptor
  {
    enum class Operation
    {
      READ,
      WRITE,
      READ_WRITE
    };

    TransactionDescriptor(Operation newOperation, std::vector<uint16_t> newSubaddress);

    bool readable{false};
    bool writable{false};
    std::vector<uint16_t> subaddresses;
  };

  using RegisterFile = uint8_t;
  using TransactionDescriptorMap = std::map<RegisterFile, TransactionDescriptor>;
  using Transaction = std::tuple<Operation, RegisterFile, std::experimental::optional<uint16_t>, std::vector<uint8_t>>;

 public:
  Application() = default;
  Application(const Application& source) = delete;
  Application(Application&& source) = delete;
  ~Application() = default;

  Application& operator=(const Application& source) = delete;
  Application& operator=(Application&& source) = delete;

 protected:
  void initialize(int stage) override;

 private:
  void handleIncommingMessage(cMessage* message) override final;

  static const TransactionDescriptorMap supportedTransactions;
};

}  // namespace decaweave
}  // namespace smile

#endif  // WITH_DECAWEAVE
