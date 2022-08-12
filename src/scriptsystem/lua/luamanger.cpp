#include <fmt/core.h>

#include <lua.hpp>

#include "log/log.h"
#include "scriptsystem/lua/luamanager.h"

namespace WhiteDeer {
namespace Engine {

int test_call_native(lua_State *L) {
  auto number = luaL_checkinteger(L, 1);
  LOGI << "call native success! number =" << number;
  return 0;
}

int LuaManager::native_class(lua_State *) { LOGE << "native in class"; return 0;}
int LuaManager::native_static(lua_State *) { LOGE << "native in class"; return 0; }

LuaManager::LuaManager() {
  L = luaL_newstate();
  luaL_openlibs(L);

  root = GetLocalFileSystem()->ToAbsolute("package/scripts/lua");
  root.make_preferred();

  // add script root to import path list
  DoString(fmt::format("package.path = package.path..[[;{0}\\?.lua]]",
                       root.string()));
  // repl facilities
  DoFile("repl.lua");

  lua_register(L, "test_call_native", LuaManager::native_static);
}

LuaManager::~LuaManager() {
  if (L != nullptr) {
    lua_close(L);
  }
}

bool LuaManager::CheckCall(int result) {
    if (result != LUA_OK) {
        LOGE << lua_tostring(L, -1);
        return false;
    }
    return true;
}

bool LuaManager::DoString(const string &code) {
  return CheckCall(luaL_dostring(L, code.c_str()));
}

string LuaManager::Repl(const string &code) {
  lua_getglobal(L, "CodeRun");
  lua_pushstring(L, code.c_str());
  CheckCall(lua_pcall(L, 1, 1, 0));
  return luaL_checkstring(L, -1);
}

bool LuaManager::DoFile(const string &filePath) {
  auto abspath = root / filePath;
  return CheckCall(luaL_dofile(L, abspath.string().c_str()));
}

}  // namespace Engine
}  // namespace WhiteDeer
