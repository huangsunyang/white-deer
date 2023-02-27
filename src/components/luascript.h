#pragma once

#include "components/script.h"
#include "filesystem/localfilesystem.h"
#include "scriptsystem/lua/luamanager.h"

namespace WhiteDeer {
namespace Engine {

class LuaScript : public Script {
  DECLARE_COMPONENT(LuaScript)

 public:
  virtual void Update() override {
    if (filename.empty()) return;
    auto lua = LuaManager::GetLua();
    auto luaModule = lua["require"](filename);
    sol::table as_table = luaModule.get<sol::table>();
    as_table["Update"]();
  }

  virtual void SetScript(const string& script) { filename = script; }

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    Script::VirtualTransfer(transfer, name);
  }

 protected:
  string filename;
};

}  // namespace Engine
}  // namespace WhiteDeer