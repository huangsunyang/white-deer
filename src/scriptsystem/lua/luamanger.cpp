#include "lua.hpp"
#include "scriptsystem/lua/luamanager.h"

namespace WhiteDeer {
namespace Engine {
LuaManager::LuaManager() {
    L = luaL_newstate();
    luaL_openlibs(L);
}
}  // namespace Engine
}  // namespace WhiteDeer
