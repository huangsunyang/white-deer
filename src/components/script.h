#pragma once

#include "components/component.h"

namespace WhiteDeer {
namespace Engine {

class Script : public Component {
  DECLARE_COMPONENT(Script)

 public:
  virtual void Update() {}

  template <typename T>
  void Transfer(T* transfer, const string& name) {}
};

}  // namespace Engine
}  // namespace WhiteDeer