#pragma once
#include <imgui.h>

#include "editor/gui/editorgui.h"
#include "editor/gui/luawindow.h"

namespace WhiteDeer {
namespace Editor {

class MainMenu : public EditorWindow<MainMenu> {
 public:
  virtual void Render() {
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("Script")) {
        ShowScriptMenu();
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

 protected:
  void ShowScriptMenu() {
    if (ImGui::MenuItem("Lua")) {
        LuaWindow::GetInstance()->Open();
    }
  }
};

}  // namespace Editor
}  // namespace WhiteDeer
