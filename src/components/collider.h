#pragma once

#include "utils/common/enum.h"
#include "components/component.h"
#include "physics/physicsmanager.h"

DECLARE_ENUM(ColliderType, ColliderType_None, ColliderType_Plane, ColliderType_Box,
             ColliderType_Sphere);

namespace WhiteDeer::Engine {
class Collider : public Component {
  DECLARE_WHITEDEER_TYPE(Collider);

 public:
  virtual void Awake() override;
  virtual void Update() override;

  virtual void OnPhysicsSimulate();

  void SetStatic(bool);
  void UseRigidBody(bool);
  void SetColliderType(ColliderType, float=0.0, float=0.0, float=0.0);

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    transfer->Transfer("type", &m_type);
    transfer->Transfer("static", &m_isStatic);
    transfer->Transfer("rigid body", &m_useRigidbody);
  }

 protected:
  void UpdateRigidBody();

 protected:
  ColliderType m_type;
  bool m_isStatic = true;
  bool m_useRigidbody = false;

  PxRigidActor* m_rigidActor;

  glm::vec3 m_box;
  float m_radius;
  glm::vec2 m_plane;

 private:
  bool m_rigidDirty = false;
};
}  // namespace WhiteDeer::Engine