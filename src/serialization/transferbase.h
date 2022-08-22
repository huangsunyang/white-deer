#pragma once

#include <glm/glm.hpp>
#include <string>

#include "log/log.h"

using std::string;

namespace WhiteDeer {
namespace Engine {

#define TRANSFER_SPECIALIZATION(type_)                                \
  template <>                                                         \
  void TransferHelper<type_>(const string& name, const type_* data) { \
    TransferBaseType(name, data);                                     \
  }

class TransferBase {
 public:
  template <typename T>
  void Transfer(const string& name, const T* data) {
    TransferHelper(name, data);
  }

  template <typename T>
  void TransferBaseType(const string& name, const T* data) {
    // base type
    // LOGE << name << ' ' << *data;
  }

 protected:
  template <typename T>
  void TransferHelper(const string& name, const T* data) {
    data->Transfer(this, name);
  }

  TRANSFER_SPECIALIZATION(int)
  TRANSFER_SPECIALIZATION(float)
  TRANSFER_SPECIALIZATION(bool)
  TRANSFER_SPECIALIZATION(std::string)
  TRANSFER_SPECIALIZATION(glm::vec3)
};

}  // namespace Engine
}  // namespace WhiteDeer
