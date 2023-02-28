#include "components/luascript.h"

namespace WhiteDeer::Engine {

void LuaScript::Awake() {
  Script::Awake();
  if (filename.empty()) return;
  auto lua = LuaManager::GetLua();
  auto luaModule = lua["require"](filename);
  auto as_table = luaModule.get<sol::table>();
  m_instance = as_table["new"](as_table);
  m_instance["__instance"] = this;

  m_instance["Awake"](m_instance);
}

void LuaScript::Update() {
  m_instance["Update"](m_instance);
}

}
