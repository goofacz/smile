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

#include "Application.h"
#include "omnetpp.h"

namespace smile {
namespace mocks {

class MockApplication : public Application
{
 public:
  MockApplication() = default;
  MockApplication(const MockApplication& source) = delete;
  MockApplication(MockApplication&& source) = delete;
  ~MockApplication() = default;

  MockApplication& operator=(const MockApplication& source) = delete;
  MockApplication& operator=(MockApplication&& source) = delete;
};

}  // namespace mocks
}  // namespace smile
