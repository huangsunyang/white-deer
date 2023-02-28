#include "components/luascript.h"

namespace WhiteDeer::Engine {

void LuaScript::Awake() {
  Script::Awake();
  if (filename.empty()) return;
  auto lua = LuaManager::GetLua();
  try {
    auto luaModule = lua["require"](filename);
    if (!luaModule.valid()) {
      sol::error err = luaModule;
      sol::call_status status = luaModule.status();
      std::cout << "Something went horribly wrong: " << sol::to_string(status)
                << " error"
                << "\n\t" << err.what() << std::endl;
      return;
    }

    auto as_table = luaModule.get<sol::table>();
    m_instance = as_table["new"](as_table);
    m_instance["__instance"] = this;
    m_instance["Awake"](m_instance);
  } catch (const sol::error& err) {
    std::cout << "Something went horribly wrong: "
                 "thrown error"
              << "\n\t" << err.what() << std::endl;
  }
}

void LuaScript::Update() { m_instance["Update"](m_instance); }
}  // namespace WhiteDeer::Engine
