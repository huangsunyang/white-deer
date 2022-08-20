#pragma once

#include <string>
#include <vector>

#include "components/component.h"
#include "utils/common/registry.h"
#include "utils/common/singleton.h"

using std::string;
using std::vector;

namespace WhiteDeer {
namespace Engine {

class GameObject {
 public:
  GameObject(const string &n) : m_name(n) {}
  Component *AddComponent(Component *comp) {
    m_components.push_back(comp);
    return comp;
  };

  template <typename T, typename... Args>
  T *AddComponent(Args... args) {
    T *ptr = new T(args...);
    AddComponent(ptr);
    return ptr;
  }

  void RemoveComponent(Component *toremove) {
    auto iter = std::remove(m_components.begin(), m_components.end(), toremove);
    for (auto it = iter; it != m_components.end(); it++) {
      delete *it;
    }
    m_components.erase(iter, m_components.end());
  }

  template <typename T>
  void RemoveComponent() {
    auto iter = std::remove_if(
        m_components.begin(), m_components.end(),
        [](const Component *element) { return element->IsInstanceOf<T>(); });
    for (auto it = iter; it != m_components.end(); it++) {
      delete *it;
    }
    m_components.erase(iter, m_components.end());
  }

  template <typename T>
  Component *GetComponent() {
    for (auto comp : m_components) {
      if (comp->IsInstanceOf<T>()) {
        return comp;
      }
    }
    return nullptr;
  }

  template <typename T>
  vector<T *> GetComponents() {
    vector<T *> comps;
    for (auto comp : m_components) {
      if (comp->IsInstanceOf<T>()) {
        comps.push_back(dynamic_cast<T*>(comp));
      }
    }
    return comps;
  }

  template <typename T>
  vector<T *> GetComponentsInChildren() {
    auto comps = GetComponents<T>();
    for (auto child : m_children) {
      auto childcomps = child->GetComponentsInChildren<T>();
      comps.insert(comps.end(), childcomps.begin(), childcomps.end());
    }
    return comps;
  }

  GameObject *NewChild() { return AddChild(new GameObject("empty")); }

  GameObject *AddChild(GameObject *o) {
    if (o->m_parent != nullptr) {
      o->m_parent->RemoveChild(o);
    }
    m_children.push_back(o);
    o->m_parent = o;
    return o;
  }

  void RemoveChild(GameObject *o) {
    auto iter = std::remove(m_children.begin(), m_children.end(), o);
    m_children.erase(iter, m_children.end());
  }

  void SetParent(GameObject *o) {
    if (o) {
      o->AddChild(this);
    }
  }

 protected:
  string m_name;
  vector<Component *> m_components;

  GameObject *m_parent = nullptr;
  vector<GameObject *> m_children;
};

// fast and rough
class Scene : public GameObject {
 public:
  Scene(const string &name) : GameObject(name) {}

 protected:
};

using Utils::StaticNamedPool;
class SceneManager : public StaticNamedPool<string, Scene> {
 public:
  static shared_ptr<Scene> GetCurrentScene() { return GetOrLoad("default"); }

 protected:
};

}  // namespace Engine
}  // namespace WhiteDeer