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

#include <cassert>
#include <functional>

#include "omnetpp.h"

namespace smile {

template <typename T>
class Listener : public omnetpp::cListener
{
 public:
  using Callback = std::function<void(omnetpp::cComponent*, omnetpp::simsignal_t, T, omnetpp::cObject*)>;

 public:
  Listener();
  explicit Listener(Callback callback);
  Listener(Listener&& source) = delete;
  ~Listener() = default;

  Listener& operator=(const Listener& source) = delete;
  Listener& operator=(Listener&& source) = delete;
  Listener& operator=(Callback newCallback);

  void receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, T value,
                     omnetpp::cObject* details) override;

 private:
  Callback callback;
};

template <typename T>
Listener<T>::Listener() : Listener(nullptr)
{}

template <typename T>
Listener<T>::Listener(Callback callback) : callback{std::move(callback)}
{}

template <typename T>
Listener<T>& Listener<T>::operator=(Callback source)
{
  callback = std::move(source);
  return *this;
}

template <typename T>
void Listener<T>::receiveSignal(omnetpp::cComponent* source, omnetpp::simsignal_t signalID, T value,
                                omnetpp::cObject* details)
{
  assert(callback);
  callback(source, signalID, value, details);
}

}  // namespace smile
