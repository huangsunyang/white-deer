#pragma once

#include "editor/gui/editorgui.h"
#include "scene/gameobject.h"
#include "serialization/editortransfer.h"

using namespace WhiteDeer::Engine;

namespace WhiteDeer {
namespace Editor {

class ObjectWindow : public EditorWindow<ObjectWindow> {
 public:
  void SetGameObject(GameObject* go) { m_gameobject = go; }

 protected:
  virtual void Render() {
    ImGui::Begin("Inspector", &m_showing);
    if (m_gameobject != nullptr) {
      auto components = m_gameobject->GetAllComponents();
      for (auto comp : components) {
        EditorTransfer transfer;
        transfer.Transfer("", comp);
      }
    }
    ImGui::End();
  }

  GameObject* m_gameobject = nullptr;
};

}  // namespace Editor
}  // namespace WhiteDeer