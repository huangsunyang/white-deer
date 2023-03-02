#pragma once

#include <string>
#include <vector>

#include "components/component.h"
#include "components/luascript.h"
#include "physics/physicsmanager.h"
#include "scene/gameobject.h"
#include "utils/common/registry.h"
#include "utils/common/singleton.h"

using std::string;
using std::vector;

using namespace physx;

namespace WhiteDeer {
namespace Engine {

// fast and rough
class Scene : public GameObject {
 public:
  Scene(const string& name);
  void Update();
  PxScene* GetPhysicsScene() { return m_physicsScene; }

 protected:
  PxScene* m_physicsScene;
};

using Utils::StaticNamedPool;
class SceneManager : public StaticNamedPool<string, Scene> {
 public:
  static shared_ptr<Scene> GetCurrentScene() { return GetOrLoad("default"); }

 protected:
};

}  // namespace Engine
}  // namespace WhiteDeer