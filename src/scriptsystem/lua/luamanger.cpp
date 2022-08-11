#include <fmt/core.h>

#include <lua.hpp>

#include "log/log.h"
#include "scriptsystem/lua/luamanager.h"

namespace WhiteDeer {
namespace Engine {

int test_call_native(lua_State * L) {
  auto number = luaL_checkinteger(L, 1);
  LOGI << "call native success! number =" << number;
  return 0;
}

LuaManager::LuaManager() {
  L = luaL_newstate();
  luaL_openlibs(L);

  root = GetLocalFileSystem()->ToAbsolute("package/scripts/lua");
  root.make_preferred();
  DoString(fmt::format("package.path = package.path..[[;{0}\\?.lua]]",
                       root.string().c_str()));

  lua_register(L, "test_call_native", test_call_native);
}

LuaManager::~LuaManager() {
  if (L != nullptr) {
    lua_close(L);
  }
}

bool LuaManager::DoString(const string &code) {
  if (luaL_dostring(L, code.c_str()) != LUA_OK) {
    LOGE << lua_tostring(L, -1);
    return false;
  }
  return true;
}

bool LuaManager::DoFile(const string &filePath) {
  auto abspath = root / filePath;
  if (luaL_dofile(L, abspath.string().c_str()) != LUA_OK) {
    LOGE << lua_tostring(L, -1);
    return false;
  }
  return true;
}

}  // namespace Engine
}  // namespace WhiteDeer
