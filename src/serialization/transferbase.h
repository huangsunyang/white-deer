#pragma once

#include <glm/glm.hpp>
#include <string>

#include "log/log.h"

using std::string;

namespace WhiteDeer {
namespace Engine {

#define TRANSFER_SPECIALIZATION(type_)                        \
  template <>                                                 \
  void TransferHelper<type_>(const char* name, type_* data) { \
    TransferBaseType(name, data);                             \
  }

class TransferBase {
 public:
  template <typename T>
  void Transfer(const char* name, T* data) {
    TransferHelper(name, data);
  }

  template <typename T>
  void TransferBaseType(const char* name, T* data) {}

 protected:
  template <typename T>
  void TransferHelper(const char* name, T* data) {
    data->Transfer(this, name);
  }
};

}  // namespace Engine
}  // namespace WhiteDeer
