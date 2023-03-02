#include "physics/physicsmanager.h"

using namespace physx;

static PxDefaultAllocator gAllocator;
static PxDefaultErrorCallback gErrorCallback;


namespace WhiteDeer::Engine {
PhysicsManager::PhysicsManager() { InitPhysics(); }

void PhysicsManager::InitPhysics() {
  gFundation =
      PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

  // physx visualize debugger
  gPvd = PxCreatePvd(*gFundation);
  PxPvdTransport* transport =
      PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
  gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

  gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFundation,
                             PxTolerancesScale(), true, gPvd);
  gDispatcher = PxDefaultCpuDispatcherCreate(2);

}

PxScene * PhysicsManager::CreateScene() {
  PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = PxVec3(0.0f, -98.1f, 0.0f);
  sceneDesc.cpuDispatcher = gDispatcher;
  sceneDesc.filterShader = PxDefaultSimulationFilterShader;
  auto scene = gPhysics->createScene(sceneDesc);

  PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
  if (pvdClient) {
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
  }

  return scene;
}

void PhysicsManager::CleanupPhysics() {
  PX_RELEASE(gDispatcher);
  PX_RELEASE(gPhysics);
  if (gPvd) {
    PxPvdTransport* transport = gPvd->getTransport();
    gPvd->release();
    gPvd = NULL;
    PX_RELEASE(transport);
  }
  PX_RELEASE(gFundation);
}

void PhysicsManager::UpdateScene(PxScene* scene, float delta) {
  scene->simulate(delta);
  scene->fetchResults(true);
}
}  // namespace WhiteDeer::Engine