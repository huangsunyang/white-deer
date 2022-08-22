#pragma once
#include <string>

using std::string;

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

  template <typename T>
  void Transfer(const T* transfer, const string& s) const {}

 protected:
  GameObject* m_gameobject;
};

}  // namespace Engine
}  // namespace WhiteDeer