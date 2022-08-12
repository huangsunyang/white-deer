#include "editor/gui/editor_gui.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>

#include "application/application.h"
#include "editor/gui/logwindow.h"
#include "editor/gui/luawindow.h"
#include "editor/gui/mainmenu.h"
#include "editor/gui/testwindow.h"
#include "editor/gui/shaderwindow.h"

namespace WhiteDeer {
namespace Editor {

using WhiteDeer::Engine::Application;

void EditorGUIManager::InitEditors() {
  MainMenu::Register();
  LogWindow::Register();
  TestWindow::Register();
  LuaWindow::Register();
  ShaderWindow::Register();
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