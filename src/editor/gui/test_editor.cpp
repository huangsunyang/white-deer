#include "imgui.h"
#include "editor/gui/test_editor.h"

namespace WhiteDeer {
namespace Editor {
void TestEditor::Render() { // 1. Show the big demo window
  if (m_show_demo_window)
    ImGui::ShowDemoWindow(&m_show_demo_window);

  // 2. Show a simple window that we create ourselves. We use a Begin/End pair
  // to created a named window.
  if (m_showing) {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!", &m_showing);
    ImGui::Text("This is some useful text."); // Display some text
    ImGui::Checkbox("Demo Window", &m_show_demo_window);
    ImGui::Checkbox("Another Window", &m_show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float *)&m_clear_color);

    if (ImGui::Button("Button"))
      counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  // 3. Show another simple window.
  if (m_show_another_window) {
    ImGui::Begin("Another Window", &m_show_another_window);
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
      m_show_another_window = false;
    ImGui::End();
  }
};
} // namespace Editor
} // namespace WhiteDeer