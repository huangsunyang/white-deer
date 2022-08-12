#pragma once

#include <string>

#include "filesystem/localfilesystem.h"
#include "lua.hpp"
#include "utils/common/singleton.h"

using std::string;
using WhiteDeer::Utils::Singleton;
namespace fs = std::filesystem;

namespace WhiteDeer {
namespace Engine {
class LuaManager : public Singleton<LuaManager> {
 public:
  friend class Singleton<LuaManager>;

  ~LuaManager();

  bool DoString(const string &code);
  bool DoFile(const string &fileName);
  string Repl(const string& code);

  // test
  static int native_static(lua_State *);
  int native_class(lua_State *);

 protected:
  LuaManager();
  bool CheckCall(int);

 protected:
  lua_State *L;
  fs::path root;
};
}  // namespace Engine
}  // namespace WhiteDeer
