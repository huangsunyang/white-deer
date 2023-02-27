#pragma once

#include "components/component.h"

namespace WhiteDeer {
namespace Engine {

class Script : public Component {
  DECLARE_WHITEDEER_TYPE(Script)

 public:
  virtual void Awake() {}
  virtual void Update() {}

  template <typename T>
  void Transfer(T* transfer, const string& name) {}
};

}  // namespace Engine
}  // namespace WhiteDeer