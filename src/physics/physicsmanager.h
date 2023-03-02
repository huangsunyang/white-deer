#pragma once
#include "PxPhysicsAPI.h"
#include "utils/common/singleton.h"

using WhiteDeer::Utils::Singleton;
using namespace physx;

namespace WhiteDeer::Engine {

class PhysicsManager : public Singleton<PhysicsManager> {
  friend class Singleton<PhysicsManager>;

 public:
  void UpdateScene(PxScene*, float = 1.0f / 60.0f);
  PxPhysics* GetPhysics() { return gPhysics; }
  PxScene* CreateScene();

 protected:
  PhysicsManager();
  void InitPhysics();
  void CleanupPhysics();

  void PhysicsManager::createStack(const PxTransform& t, PxU32 size,
                                   PxReal halfExtent);
  PxRigidDynamic* PhysicsManager::createDynamic(
      const PxTransform& t, const PxGeometry& geometry,
      const PxVec3& velocity = PxVec3(0));

 protected:
  PxFoundation* gFundation = nullptr;
  PxPvd* gPvd = nullptr;
  PxPhysics* gPhysics = nullptr;
  PxDefaultCpuDispatcher* gDispatcher = nullptr;
};

}  // namespace WhiteDeer::Engine