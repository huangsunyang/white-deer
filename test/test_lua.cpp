#include "catch2/catch_test_macros.hpp"
#include "scriptsystem/lua/luamanager.h"
#include <sol/sol.hpp>

using namespace WhiteDeer::Engine;

TEST_CASE("lua script", "[lua]") {
    REQUIRE(LuaManager::GetInstance()->DoString("print('hello world from lua!')"));
    REQUIRE(LuaManager::GetInstance()->DoString("print(package.path)"));
    REQUIRE(!LuaManager::GetInstance()->DoString("print(package.path)a"));
    REQUIRE(LuaManager::GetInstance()->DoFile("main.lua"));
    REQUIRE(LuaManager::GetInstance()->Repl("\"main.lua\"") == "\"main.lua\"");
}

