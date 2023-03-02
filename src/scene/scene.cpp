#include "scene/scene.h"
#include "components/collider.h"


namespace WhiteDeer {
namespace Utils {

using Engine::Scene;
using Engine::SceneManager;
SceneManager::CONTAINER SceneManager::s_entries;

}  // namespace Utils

namespace Engine {

Scene::Scene(const string& name) : GameObject(name) {
  auto physicMgr = PhysicsManager::GetInstance();
  m_physicsScene = physicMgr->CreateScene();
}

void Scene::Update() {
  auto components = GetComponentsInChildren<Component>();
  for (auto comp : components) {
    if (!comp->IsInitialized()) comp->Awake();
    else comp->Update();
  }

  // update physics
  auto physicMgr = PhysicsManager::GetInstance();
  physicMgr->UpdateScene(m_physicsScene);

  auto colliders = GetComponentsInChildren<Collider>();
  for (auto collider: colliders) {
    collider->OnPhysicsSimulate();
  }
}

}  // namespace Engine

}  // namespace WhiteDeer