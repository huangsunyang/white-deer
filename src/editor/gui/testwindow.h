#pragma once
#include <imgui.h>

#include <glm/glm.hpp>

#include "camera/camera.h"
#include "editor/gui/editorgui.h"
#include "graphics/opengl/glshader.h"

using namespace WhiteDeer::Graphics;
using namespace WhiteDeer::Engine;

namespace WhiteDeer {
namespace Editor {

class TestWindow : public EditorWindow<TestWindow> {
 protected:
  bool m_show_demo_window = true;
  bool m_show_another_window = false;
  ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  virtual void Render() {
    if (m_show_demo_window) ImGui::ShowDemoWindow(&m_show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to created a named window.
    if (m_showing) {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!", &m_showing);
      ImGui::Text("This is some useful text.");  // Display some text
      ImGui::Checkbox("Demo Window", &m_show_demo_window);
      ImGui::Checkbox("Another Window", &m_show_another_window);

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3("clear color", (float *)&m_clear_color);

      if (ImGui::Button("Reload Shader")) {
        counter++;
        Program::RefreshAll();
      }
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

      Camera *camera = CameraManager::GetInstance()->GetDefaultCamera();
      if (camera) {
        // ImGui::SliderFloat3("target pos", &camera->GetTargetPos()[0], -1.0f, 1.0f);
        ImGui::SliderFloat3("camera pos", &camera->GetPos()[0], -1.0f, 1.0f);
      }
      ImGui::End();
    }

    // 3. Show another simple window.
    if (m_show_another_window) {
      ImGui::Begin("Another Window", &m_show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me")) m_show_another_window = false;
      ImGui::End();
    }
  }
};
}  // namespace Editor
}  // namespace WhiteDeer