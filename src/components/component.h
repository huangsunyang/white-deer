#pragma once
#include <string>

using std::string;

#define DECLARE_COMPONENT(name) \
 public:                        \
  virtual string GetComponentName() { return #name; }

namespace WhiteDeer {
namespace Engine {

class GameObject;
class TransferBase;

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

  GameObject* GetGameObject() { return m_gameobject; }

 protected:
  GameObject* m_gameobject;
};

}  // namespace Engine
}  // namespace WhiteDeer