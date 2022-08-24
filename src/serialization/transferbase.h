#pragma once

#include <glm/glm.hpp>
#include <string>

#include "log/log.h"

using std::string;

namespace WhiteDeer {
namespace Engine {

class TransferBase {
 public:
  template <typename T>
  void Transfer(const string& name, T* data) {
    data->Transfer(this, name);
  }
};

}  // namespace Engine
}  // namespace WhiteDeer
