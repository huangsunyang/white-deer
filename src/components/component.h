#pragma once
#include <string>
#include "serialization/editortransfer.h"

using std::string;

namespace WhiteDeer {
namespace Engine {

class GameObject;
class TransferBase;

#define DECLARE_COMPONENT(name)                                         \
 public:                                                                \
  virtual string GetComponentName() { return #name; }                   \
  virtual void VirtualTransfer(EditorTransfer* t, const string& name) { \
    Transfer(t, name);                                                  \
  }

class Component {
  DECLARE_COMPONENT(Component)
  friend class GameObject;

 public:
  virtual ~Component() {}

  template <typename T>
  bool IsInstanceOf() {
    return Cast<T>() != nullptr;
  }

  template <typename T>
  T* Cast() {
    return dynamic_cast<T*>(this);
  }

  template <typename T>
  void Transfer(T* transfer, const string& name) {}

  GameObject* GetGameObject() { return m_gameobject; }

 protected:
  GameObject* m_gameobject;
};

}  // namespace Engine
}  // namespace WhiteDeer