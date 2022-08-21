#pragma once

namespace WhiteDeer {
namespace Engine {

class GameObject;

class Component {
  friend class GameObject;

 public:
  virtual ~Component() {}

  template <typename T>
  bool IsInstanceOf() {
    return dynamic_cast<const T*>(this) != nullptr;
  }

  GameObject* GetGameObject() { return m_gameobject; }

 protected:
  GameObject* m_gameobject;
};

}  // namespace Engine
}  // namespace WhiteDeer