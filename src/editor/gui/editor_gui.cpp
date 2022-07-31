#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>

#include "application/application.h"
#include "editor/gui/editor_gui.h"
#include "editor/gui/test_editor.h"

namespace WhiteDeer {
namespace Editor {

using WhiteDeer::Engine::Application;

void EditorGUIManager::InitEditors() {
  EditorRegistry<TestEditor>::Register();
}

void EditorGUIManager::Render() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // render all registered editors
  for (auto editor: m_editorGUIVec) {
    editor->Render();
  }

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorGUIManager::RegisterEditor(EditorGUI * editor) {
  if (!HasEditor(editor)) {
    m_editorGUIVec.push_back(editor);
  }
}

void EditorGUIManager::UnRegisterEditor(EditorGUI * editor) {
  if (HasEditor(editor)) {
    auto& vec = m_editorGUIVec;
    vec.erase(std::remove(vec.begin(), vec.end(), editor), vec.end());
  }
}

bool EditorGUIManager::HasEditor(EditorGUI * editor) {
  return std::find(m_editorGUIVec.begin(), m_editorGUIVec.end(), editor) != m_editorGUIVec.end();
}

} // namespace Editor
} // namespace WhiteDeer