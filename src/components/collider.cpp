#include "components/collider.h"

#include "scene/gameobject.h"
#include "scene/scene.h"

namespace WhiteDeer::Engine {
void Collider::Awake() { UpdateRigidBody(); }

void Collider::Update() {
  if (m_rigidDirty) {
    UpdateRigidBody();
    m_rigidDirty = false;
  }
}

void Collider::OnPhysicsSimulate() {
  if (!m_rigidActor) return;

  auto gameObject = GetGameObject();
  if (!gameObject) return;

  auto transform = gameObject->GetComponent<Transform>();
  if (!transform) return;

  auto physicsScene = SceneManager::GetCurrentScene()->GetPhysicsScene();
  auto pxpose = m_rigidActor->getGlobalPose();
  transform->SetPosition(pxpose.p.x, pxpose.p.y, pxpose.p.z);
}

void Collider::SetStatic(bool _static) {
  if (_static == m_isStatic) return;
  m_isStatic = _static;
  m_rigidDirty = true;
}

void Collider::SetColliderType(ColliderType type, float x, float y, float z) {
  if (m_type == type) return;
  m_type = type;
  if (m_type == ColliderType_Sphere) m_radius = x;
  if (m_type == ColliderType_Box) m_box = glm::vec3(x, y, z);
  if (m_type == ColliderType_Plane) m_plane = glm::vec2(x, y);
  m_rigidDirty = true;
}

void Collider::UseRigidBody(bool use) {
  if (use == m_useRigidbody) return;
  m_useRigidbody = use;
  m_rigidDirty = true;
}

void Collider::UpdateRigidBody() {
  if (m_type == ColliderType_None) return;

  auto gameObject = GetGameObject();
  if (!gameObject) return;

  auto transform = gameObject->GetComponent<Transform>();
  if (!transform) return;

  if (m_rigidActor) {
    m_rigidActor->release();
  }

  auto pos = transform->GetPosition();
  PxTransform pxpos(pos.x, pos.y, pos.z);
  auto physics = PhysicsManager::GetInstance()->GetPhysics();

  // todo: config in editor ui
  auto material = physics->createMaterial(0.5, 0.5, 0.5);
  PxShape* shape = nullptr;

  if (m_type == ColliderType_Box) {
    shape = physics->createShape(PxBoxGeometry(m_box.x, m_box.y, m_box.z),
                                 *material);
  } else if (m_type == ColliderType_Plane) {
    shape = physics->createShape(PxPlaneGeometry(), *material);
  } else if (m_type = ColliderType_Sphere) {
    shape = physics->createShape(PxSphereGeometry(m_radius), *material);
  }

  if (m_isStatic) {
    m_rigidActor = physics->createRigidStatic(pxpos);
  } else {
    auto rigidBody = physics->createRigidDynamic(pxpos);
    PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
    m_rigidActor = rigidBody;
  }

  m_rigidActor->attachShape(*shape);

  auto physicsScene = SceneManager::GetCurrentScene()->GetPhysicsScene();
  physicsScene->addActor(*m_rigidActor);
}
}  // namespace WhiteDeer::Engine