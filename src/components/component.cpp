#include "components/luascript.h"
#include "scene/gameobject.h"

#include "scriptsystem/lua/luamanager.h"

namespace WhiteDeer::Engine {

class InitializerHook {
 public:
  InitializerHook() {
    auto lua = LuaManager::GetLua();
    auto gameobject = lua.new_usertype<GameObject>(
        "GameObject", sol::constructors<GameObject(const string&)>(),
        "GetName", &GameObject::GetName
    );

    auto component = lua.new_usertype<Component>(
        "Component", sol::constructors<Component()>(),
        "GetGameObject", &Component::GetGameObject
    );

    auto scriptComp = lua.new_usertype<Script>(
        "Script", sol::constructors<Script()>(),
        sol::base_classes, sol::bases<Component>()
    );

    auto luascriptComp = lua.new_usertype<LuaScript>(
        "LuaScript", sol::constructors<LuaScript()>(),
        sol::base_classes, sol::bases<Script, Component>()
    );
  }
};

static InitializerHook hook;

}  // namespace WhiteDeer::Engine
