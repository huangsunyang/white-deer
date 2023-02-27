#pragma once

#include "components/component.h"

namespace WhiteDeer {
namespace Engine {

class Script : public Component {
  DECLARE_WHITEDEER_TYPE(Script)

 public:
  virtual void Awake() { m_initialized = true; }
  virtual void Update() {}
  virtual bool IsInitialized() { return m_initialized; }

  template <typename T>
  void Transfer(T* transfer, const string& name) {}

 protected:
  bool m_initialized = false;
};

}  // namespace Engine
}  // namespace WhiteDeer