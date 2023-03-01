#include "physics/physicsmanager.h"

#include "PxPhysicsAPI.h"

using namespace physx;

static PxDefaultAllocator gAllocator;
static PxDefaultErrorCallback gErrorCallback;
static PxFoundation* gFundation = nullptr;
static PxPvd* gPvd = nullptr;

static PxPhysics* gPhysics = nullptr;
static PxScene* gScene = nullptr;
static PxDefaultCpuDispatcher* gDispatcher = nullptr;
static PxMaterial* gMaterial = nullptr;

static PxReal stackZ = 10.0f;

namespace WhiteDeer::Engine {
PhysicsManager::PhysicsManager() { InitPhysics(); }

void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent) {
  PxShape* shape = gPhysics->createShape(
      PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
  for (PxU32 i = 0; i < size; i++) {
    for (PxU32 j = 0; j < size - 1; j++) {
      PxTransform localTm(
          PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) *
          halfExtent);
      PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
      body->attachShape(*shape);
      PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
      gScene->addActor(*body);
    }
  }
  shape->release();
}

static PxRigidDynamic* createDynamic(const PxTransform& t,
                                     const PxGeometry& geometry,
                                     const PxVec3& velocity = PxVec3(0)) {
  PxRigidDynamic* dynamic =
      PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
  dynamic->setAngularDamping(0.5f);
  dynamic->setLinearVelocity(velocity);
  gScene->addActor(*dynamic);
  return dynamic;
}

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

  PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
  sceneDesc.cpuDispatcher = gDispatcher = PxDefaultCpuDispatcherCreate(2);
  sceneDesc.filterShader = PxDefaultSimulationFilterShader;
  gScene = gPhysics->createScene(sceneDesc);

  PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
  if (pvdClient) {
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
  }

  gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);
  PxRigidStatic* groundPlane =
      PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
  gScene->addActor(*groundPlane);

  for (PxU32 i = 0; i < 5; i++) {
    createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
  }

  createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10),
                PxVec3(0, -50, -100));
}

void PhysicsManager::CleanupPhysics() {
  PX_RELEASE(gScene);
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

void PhysicsManager::Update() {
  gScene->simulate(1.0f / 60.0f);
  gScene->fetchResults(true);
}
}  // namespace WhiteDeer::Engine