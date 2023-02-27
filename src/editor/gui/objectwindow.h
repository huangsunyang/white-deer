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

      char tempName[32];
      strcpy_s(tempName, m_gameobject->GetName().c_str());
      ImGui::InputText("name", tempName, 32);
      m_gameobject->SetName(tempName);

      int sameNameCount = 0;
      for (int i = 0; i < components.size(); i++) {
        EditorTransfer transfer;
        auto name = components[i]->GetTypeName();

        // Light, Light1, Ligh2...
        if (i > 0 && components[i]->GetTypeName() ==
                         components[i - 1]->GetTypeName()) {
          sameNameCount += 1;
          name += std::to_string(sameNameCount);
        } else {
          sameNameCount = 0;
        }
        transfer.TransferComponent(name, components[i]);
      }
    }
    ImGui::End();
  }

  GameObject* m_gameobject = nullptr;
};

}  // namespace Editor
}  // namespace WhiteDeer