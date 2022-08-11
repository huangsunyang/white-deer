#include "catch2/catch_test_macros.hpp"
#include "scriptsystem/lua/luamanager.h"

using namespace WhiteDeer::Engine;

TEST_CASE("lua script", "[lua]") {
    LuaManager::GetInstance();
}

