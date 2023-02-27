#include "components/luascript.h"

namespace WhiteDeer::Engine {

void LuaScript::Awake() {
  if (filename.empty()) return;
  auto lua = LuaManager::GetLua();
  auto luaModule = lua["require"](filename);
  auto as_table = luaModule.get<sol::table>();
  m_instance = lua.create_table();
  auto metatable = lua.create_table();
  metatable["__index"] = as_table;
  lua["setmetatable"](m_instance, metatable);
}

void LuaScript::Update() {
  m_instance["Update"]();
}

}
