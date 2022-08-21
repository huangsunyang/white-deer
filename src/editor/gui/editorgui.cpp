#include "editor/gui/editorgui.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>

#include "application/application.h"
#include "editor/gui/gamewindow.h"
#include "editor/gui/logwindow.h"
#include "editor/gui/luawindow.h"
#include "editor/gui/mainmenu.h"
#include "editor/gui/shaderwindow.h"
#include "editor/gui/testwindow.h"
#include "editor/gui/scenewindow.h"
#include "editor/gui/objectwindow.h"
#include "filesystem/localfilesystem.h"

namespace WhiteDeer {
namespace Editor {

using WhiteDeer::Engine::Application;
using WhiteDeer::Engine::GetLocalFileSystem;

void EditorGUIManager::InitEditors() {
  ImGuiIO &io = ImGui::GetIO();
  auto fontPath = GetLocalFileSystem()->ToAbsolute("c:/windows/fonts/deng.ttf");
  LOGD.printf("loading font: %s", fontPath.string().c_str());
  io.Fonts->AddFontFromFileTTF(
      fontPath.string().c_str(), 18, nullptr,
      io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  MainMenu::Register();
  TestWindow::Register();
  ShaderWindow::Register();
  LogWindow::Register();
  LuaWindow::Register();
  GameWindow::Register();
  SceneWindow::Register();
  ObjectWindow::Register();
}

void EditorGUIManager::Render() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // full screen dock space
  ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

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