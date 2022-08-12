
#include "editor/gui/luawindow.h"

#include <imgui.h>

#include "log/log.h"
#include "scriptsystem/lua/luamanager.h"
#include "ticktime/timemanager.h"

using WhiteDeer::Engine::LuaManager;
using WhiteDeer::Engine::TimeManager;

namespace WhiteDeer {
namespace Editor {
void LuaWindow::Render() {
  if (!m_showing) {
    return;
  }

  auto timemgr = TimeManager::GetInstance();
  auto luamgr = LuaManager::GetInstance();
  ImGui::Begin("LuaWindow");

  static long long lastRecordTime = 0;
  static string luaMem;

  auto currentTime = timemgr->GetGameTime();
  if (lastRecordTime == 0 || currentTime - lastRecordTime > 1000) {
    luaMem = luamgr->Repl("collectgarbage('count')");
    lastRecordTime = currentTime;
    LOGE << luaMem;
  }

  ImGui::Text("Lua Memory: %s", luaMem.c_str());
  ImGui::End();
}
}  // namespace Editor
}  // namespace WhiteDeer
