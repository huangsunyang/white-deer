#pragma once

#include "editor/gui/editor_gui.h"
#include "graphics/opengl/glshader.h"

using namespace WhiteDeer::Graphics;

using std::string;

namespace WhiteDeer {
namespace Editor {

class ShaderWindow : public EditorWindow<ShaderWindow> {
 public:
  friend class Singleton<ShaderWindow>;
  virtual void Render() {
    if (ImGui::Begin("Shader Window", &m_showing)) {
      if (ImGui::Button("Reload Shader")) {
        Program::RefreshAll();
      }
      ImGui::End();
    }
  };
};

}  // namespace Editor
}  // namespace WhiteDeer