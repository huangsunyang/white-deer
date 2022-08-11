#pragma once

#include "lua.hpp"
#include "utils/common/singleton.h"

using WhiteDeer::Utils::Singleton;

namespace WhiteDeer {
namespace Engine {
class LuaManager : public Singleton<LuaManager> {
  friend class Singleton<LuaManager>;

 protected:
  LuaManager();

 protected:
  lua_State* L;
};
}  // namespace Engine
}  // namespace WhiteDeer
