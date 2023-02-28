#include <fmt/core.h>

#include <lua.hpp>
#include <sol/sol.hpp>

#include "log/log.h"
#include "scriptsystem/lua/luamanager.h"

namespace WhiteDeer {
namespace Engine {

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

int my_exception_handler(lua_State* L,
     sol::optional<const std::exception&> maybe_exception,
     sol::string_view description) {
	// L is the lua state, which you can wrap in a state_view if
	// necessary maybe_exception will contain exception, if it
	// exists description will either be the what() of the
	// exception or a description saying that we hit the
	// general-case catch(...)
	std::cout << "An exception occurred in a function, here's "
	             "what it says ";
	if (maybe_exception) {
		std::cout << "(straight from the exception): ";
		const std::exception& ex = *maybe_exception;
		std::cout << ex.what() << std::endl;
	}
	else {
		std::cout << "(from the description parameter): ";
		std::cout.write(description.data(),
		     static_cast<std::streamsize>(description.size()));
		std::cout << std::endl;
	}

	// you must push 1 element onto the stack to be
	// transported through as the error object in Lua
	// note that Lua -- and 99.5% of all Lua users and libraries
	// -- expects a string so we push a single string (in our
	// case, the description of the error)
	return sol::stack::push(L, description);
}

LuaManager::LuaManager() {
  L = luaL_newstate();
  luaL_openlibs(L);
  sol::state_view lua(L);

  root = GetLocalFileSystem()->ToAbsolute("package/scripts/lua");
  root.make_preferred();

  lua.set_exception_handler(&my_exception_handler);

  // add script root to import path list
  lua.script(fmt::format("package.path = [[{0}\\?.lua;]]..package.path",
                       root.string()));
  lua.script(fmt::format("package.cpath = [[{0}\\?.dll;]]..package.cpath",
                       root.string()));
  // repl/hotreload facilities
  lua["GetFileLastModifiedTimestamp"] = [](string s) {
    std::filesystem::path p(s);
    if (!std::filesystem::exists(p)) {
        return 0;
    }
    auto last_write_time = std::filesystem::last_write_time(p);
    return (int)last_write_time.time_since_epoch().count();
  };

  lua["prints"] = [](std::string s) {
    LOGE << s;
  };
  lua["script_root_path"] = root.string();
  lua.script("require 'repl'");
  lua.script("require 'hotreload'");
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
