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
        "GetName", &GameObject::GetName,
        "GetTransformComponent", &GameObject::GetComponent<Transform>
    );

    auto vec3 = lua.new_usertype<glm::vec3>(
        "vec3", sol::constructor_list<glm::vec3(), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z
    );

    auto transformComp = lua.new_usertype<Transform>(
        "Transform", sol::constructors<Transform()>(),
        "position", sol::property(&Transform::GetPosition, sol::resolve<void(glm::vec3)>(&Transform::SetPosition)),
        "rotation", sol::property(&Transform::GetRotation, sol::resolve<void(glm::vec3)>(&Transform::SetRotation)),
        "scale", sol::property(&Transform::GetScale, sol::resolve<void(glm::vec3)>(&Transform::SetScale))
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
        "filename", sol::property(&LuaScript::GetFileName, &LuaScript::SetFileName),
        sol::base_classes, sol::bases<Script, Component>()
    );
  }
};

static InitializerHook hook;

}  // namespace WhiteDeer::Engine
