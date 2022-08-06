#include "editor/gui/editor_gui.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>

#include "application/application.h"
#include "editor/gui/test_editor.h"
#include "editor/gui/logwindow.h"

namespace WhiteDeer {
namespace Editor {

using WhiteDeer::Engine::Application;

void EditorGUIManager::InitEditors() {
    EditorRegistry<LogWindow>::Register();
    EditorRegistry<TestEditor>::Register();
}

void EditorGUIManager::Render() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // render all registered editors
  for (auto editor : m_entries) {
    editor->Render();
  }

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}  // namespace Editor
}  // namespace WhiteDeer