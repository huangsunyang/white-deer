#include <fmt/core.h>

#include <lua.hpp>
#include <sol/sol.hpp>

#include "log/log.h"
#include "scriptsystem/lua/luamanager.h"

namespace WhiteDeer {
namespace Engine {

struct player {
 public:
  int bullets;
  int speed;

  player() : player(3, 100) {}
  player(int ammo) : player(ammo, 100) {}
  player(int ammo, int hitpoints) : bullets(ammo), hp(hitpoints) {}

  void boost() { speed += 10; }
  bool shoot() {
    if (bullets < 1) return false;
    --bullets;
    return true;
  }

  void set_hp(int value) { hp = value; }
  int get_hp() const { return hp; }

 private:
  int hp;
};

int test_call_native(lua_State *L) {
  auto number = luaL_checkinteger(L, 1);
  LOGI << "call native success! number =" << number;
  return 0;
}

int LuaManager::native_class(lua_State *) {
  LOGE << "native in class";
  return 0;
}
int LuaManager::native_static(lua_State *) {
  LOGE << "native in class";
  return 0;
}

LuaManager::LuaManager() {
  L = luaL_newstate();
  luaL_openlibs(L);

  root = GetLocalFileSystem()->ToAbsolute("package/scripts/lua");
  root.make_preferred();

  // add script root to import path list
  DoString(fmt::format("package.path = [[{0}\\?.lua;]]..package.path",
                       root.string()));
  DoString(fmt::format("package.cpath = [[{0}\\?.dll;]]..package.cpath",
                       root.string()));
  // repl facilities
  DoFile("repl.lua");

  sol::state_view lua(L);
  lua.script("print('Hello sol2')");

  lua["p"] = player();
  lua["q"] = new player();

  lua_register(L, "test_call_native", LuaManager::native_static);

  sol::usertype<player> player_type = lua.new_usertype<player>(
      "player",
      // 3 constructors
      sol::constructors<player(), player(int), player(int, int)>());

  // typical member function that returns a variable
  player_type["shoot"] = &player::shoot;
  // typical member function
  player_type["boost"] = &player::boost;

  // gets or set the value using member variable syntax
  player_type["hp"] = sol::property(&player::get_hp, &player::set_hp);

  // read and write variable
  player_type["speed"] = &player::speed;
  // can only read from, not write to
  // .set(foo, bar) is the same as [foo] = bar;
  player_type.set("bullets", sol::readonly(&player::bullets));
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
