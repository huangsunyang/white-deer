#pragma once

#include "editor/gui/editorgui.h"
#include "editor/gui/textscroll.h"
#include "scriptsystem/lua/luamanager.h"
#include "ticktime/timemanager.h"

using std::string;
using WhiteDeer::Engine::LuaManager;
using WhiteDeer::Engine::TimeManager;

namespace WhiteDeer {
namespace Editor {

class LuaWindow : public EditorWindow<LuaWindow> {
 public:
  friend class Singleton<LuaWindow>;
  virtual void Render() {
    if (m_showing) {
      ImGui::Begin("Lua Window", &m_showing);
      ShowLuaGCMemInfo();
      ShowHotReloadButton();
      ShowInputField();
      m_textscroll.Draw("Lua Window", &m_showing);
      ImGui::End();
    }
  };

 protected:
  void ShowLuaGCMemInfo() {
    // lua gc memory
    static double lastRecordTime = 0;
    static string luaMem;

    auto currentTime = TimeManager::GetInstance()->GetGameTime();
    if (lastRecordTime == 0 || currentTime - lastRecordTime > 1000) {
      luaMem = LuaManager::GetInstance()->Repl(
          "tonumber(string.format('%.3f', collectgarbage('count')))");
      lastRecordTime = currentTime;
    }
    ImGui::Text("Lua Memory: %s KB", luaMem.c_str());
  }

  void ShowHotReloadButton() {
    if (ImGui::Button("Hot Reload")) {
        auto lua = LuaManager::GetLua();
        auto result = lua.script("require('hotreload').reload()", sol::script_pass_on_error);
        if (!result.valid()) {
            sol::error err = result;
			sol::call_status status = result.status();
			std::cout << "Something went horribly wrong: "
			          << sol::to_string(status) << " error"
			          << "\n\t" << err.what() << std::endl;
        }
    }
  }

  void ShowInputField() {
    static char text[1024];
    static int index = 0;

    if (ImGui::InputText("string", text, IM_ARRAYSIZE(text),
                         ImGuiInputTextFlags_CallbackCompletion |
                             ImGuiInputTextFlags_EnterReturnsTrue,
                         OnInputCallback)) {
      auto result = LuaManager::GetInstance()->Repl(text);
      m_textscroll.AddLog("[%d] %s\n", ++index, text);
      m_textscroll.AddLog("%s\n\n", result.c_str());
      memset(text, 0, 1024);

      // remain focus on text field after press enter
      ImGui::SetKeyboardFocusHere(-1);
    }
  }

  static int OnInputCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventKey == ImGuiKey_Tab) {
      LOGE << "tab pressed" << data->Buf << data->BufSize << data->BufTextLen;
    }
    return 0;
  };

  TextScroll m_textscroll;
};

}  // namespace Editor
}  // namespace WhiteDeer