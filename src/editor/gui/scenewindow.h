#pragma once
#include "editor/gui/editorgui.h"
#include "editor/gui/objectwindow.h"
#include "scene/scene.h"

using namespace WhiteDeer::Engine;

namespace WhiteDeer {
namespace Editor {

class SceneWindow : public EditorWindow<SceneWindow> {
 protected:
  virtual void Render() {
    if (!m_showing) {
      return;
    }

    ImGui::Begin("Scene", &m_showing);
    RecursiveTreeNode(SceneManager::GetCurrentScene().get(), true);
    ImGui::End();
  }

  void RecursiveTreeNode(GameObject* root, bool is_root = false) {
    ImGuiTreeNodeFlags root_flags = ImGuiTreeNodeFlags_DefaultOpen;
    ImGuiTreeNodeFlags leaf_flags =
        ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;

    auto children = root->GetChildren();
    auto flags = is_root ? root_flags : (children.empty() ? leaf_flags : 0);

    if (ImGui::TreeNodeEx(root->GetName().c_str(), flags)) {
      for (auto& child : children) {
        RecursiveTreeNode(child);
      }

      if (ImGui::IsItemClicked()) {
        ObjectWindow::GetInstance()->SetGameObject(root);
      }

      ImGui::TreePop();
    }
  }
};

}  // namespace Editor
}  // namespace WhiteDeer