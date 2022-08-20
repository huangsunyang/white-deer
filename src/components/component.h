#pragma once

namespace WhiteDeer {
namespace Engine {

class Component {
 public:
  virtual ~Component() {}

  template <typename T>
  bool IsInstanceOf() {
    return dynamic_cast<const T*>(this) != nullptr;
  }
};

}  // namespace Engine
}  // namespace WhiteDeer