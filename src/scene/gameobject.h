#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "components/component.h"
#include "components/transform.h"

using std::string;
using std::vector;

namespace WhiteDeer {
namespace Engine {

class GameObject {
 public:
  GameObject(const string &n) : m_name(n) { AddComponent<Transform>(); }

  inline Component *AddComponent(Component *comp);

  template <typename T, typename... Args>
  T *AddComponent(Args... args);

  inline void RemoveComponent(Component *toremove);

  template <typename T>
  void RemoveComponent();

  template <typename T>
  T *GetComponent();

  template <typename T>
  vector<T *> GetComponents();

  template <typename T>
  vector<T *> GetComponentsInChildren();

  inline auto GetName() { return m_name; }
  inline void SetName(const string &s) { m_name = s; }
  inline auto GetChildren() { return m_children; }
  inline auto GetAllComponents() { return m_components; }
  inline GameObject *AddChild(const string &name = "");
  inline GameObject *AddChild(GameObject *o);
  inline void RemoveChild(GameObject *o);
  inline void SetParent(GameObject *o);

 protected:
  string m_name;
  vector<Component *> m_components;

  GameObject *m_parent = nullptr;
  vector<GameObject *> m_children;
};

Component *GameObject::AddComponent(Component *comp) {
  m_components.push_back(comp);
  std::sort(m_components.begin(), m_components.end(),
            [](Component *x, Component *y) {
              return x->GetComponentName() < y->GetComponentName();
            });
  comp->m_gameobject = this;
  return comp;
};

template <typename T, typename... Args>
T *GameObject::AddComponent(Args... args) {
  T *ptr = new T(args...);
  AddComponent(ptr);
  return ptr;
}

void GameObject::RemoveComponent(Component *toremove) {
  auto iter = std::remove(m_components.begin(), m_components.end(), toremove);
  for (auto it = iter; it != m_components.end(); it++) {
    delete *it;
  }
  m_components.erase(iter, m_components.end());
}

template <typename T>
void GameObject::RemoveComponent() {
  auto iter = std::remove_if(
      m_components.begin(), m_components.end(),
      [](const Component *element) { return element->IsInstanceOf<T>(); });
  for (auto it = iter; it != m_components.end(); it++) {
    delete *it;
  }
  m_components.erase(iter, m_components.end());
}

template <typename T>
T *GameObject::GetComponent() {
  for (auto comp : m_components) {
    if (comp->IsInstanceOf<T>()) {
      return dynamic_cast<T *>(comp);
    }
  }
  return nullptr;
}

template <typename T>
vector<T *> GameObject::GetComponents() {
  vector<T *> comps;
  for (auto comp : m_components) {
    if (comp->IsInstanceOf<T>()) {
      comps.push_back(dynamic_cast<T *>(comp));
    }
  }
  return comps;
}

template <typename T>
vector<T *> GameObject::GetComponentsInChildren() {
  auto comps = GetComponents<T>();
  for (auto child : m_children) {
    auto childcomps = child->GetComponentsInChildren<T>();
    comps.insert(comps.end(), childcomps.begin(), childcomps.end());
  }
  return comps;
}

GameObject *GameObject::AddChild(const string &name) {
  return AddChild(new GameObject(name));
}

GameObject *GameObject::AddChild(GameObject *o) {
  if (o->m_parent != nullptr) {
    o->m_parent->RemoveChild(o);
  }
  m_children.push_back(o);
  o->m_parent = o;
  return o;
}

void GameObject::RemoveChild(GameObject *o) {
  auto iter = std::remove(m_children.begin(), m_children.end(), o);
  m_children.erase(iter, m_children.end());
}

void GameObject::SetParent(GameObject *o) {
  if (o) {
    o->AddChild(this);
  }
}

}  // namespace Engine
}  // namespace WhiteDeer