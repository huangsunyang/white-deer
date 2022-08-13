#pragma once

#include <fmt/core.h>

#include "editor/gui/editor_gui.h"
#include "graphics/opengl/glshader.h"

using namespace WhiteDeer::Graphics;

using std::string;

namespace WhiteDeer {
namespace Editor {

class ShaderWindow : public EditorWindow<ShaderWindow> {
 protected:
  friend class Singleton<ShaderWindow>;
  virtual void Render() {
    if (!m_showing) {
      return;
    }

    ImGui::Begin("Shader Window", &m_showing);
    if (ImGui::Button("Reload Shader")) {
      Program::RefreshAll();
    }

    ShowShaderTreeList();
    ImGui::End();
  }

  virtual void ShowShaderTreeList() {
    ImGuiTreeNodeFlags root_flags = ImGuiTreeNodeFlags_DefaultOpen;

    // Program Tree Lists
    if (ImGui::TreeNodeEx("Shader Programs", root_flags)) {
      auto& programs = Program::GetAllPrograms();
      for (auto& p_program : programs) {
        auto text = fmt::format("Program {0}", p_program->GetHandle());
        if (ImGui::TreeNodeEx(text.c_str())) {
          for (auto& p_shader : p_program->GetAllShaders()) {
            DrawShaderNode(p_shader);
          }
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }

    // Shader Tree Lists
    if (ImGui::TreeNodeEx("Shaders", root_flags)) {
      auto& shaders = Shader::GetAllShaders();
      for (auto& pair : shaders) {
        DrawShaderNode(pair.second);
      }
      ImGui::TreePop();
    }
  }

  void DrawShaderNode(const shared_ptr<Shader>& p_shader) {
    ImGuiTreeNodeFlags leaf_flags =
        ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    auto text =
        fmt::format("{0} [{1}]", p_shader->GetName(), p_shader->GetHandle());
    ImGui::TreeNodeEx(text.c_str(), leaf_flags);
    if (ImGui::IsItemClicked()) {
      LOGE << "select leaf " << p_shader->GetName();
    }
    ImGui::TreePop();
  }
};

}  // namespace Editor
}  // namespace WhiteDeer