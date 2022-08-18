#pragma once

#include <utils/common/singleton.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace WhiteDeer {
namespace Engine {

using WhiteDeer::Utils::Singleton;

class Camera {
 public:
  glm::vec3 GetZDirection();
  glm::vec3 GetYDirection();
  glm::vec3 GetXDirection();
  glm::mat4 GetViewMatrix();
  glm::mat4 GetProjectionMatrix();

  void SetPos(glm::vec3 p) { m_pos = p; }
  void SetAspect(float aspect) { m_aspect = aspect; }
  void SetTargetPos(glm::vec3 p) { m_target = p; }
  glm::vec3& GetPos() { return m_pos; }
  glm::vec3& GetTargetPos() { return m_target; }

 protected:
  glm::vec3 m_pos = {0, 0, 1};
  glm::vec3 m_target = {0, 0, 0};
  float m_fov = 45.0f;
  float m_aspect = 1920.0f / 1080.0f;

  float m_speed = 0.1f;

  // todo: camera move mode
  bool m_lockTarget = false;
};

class CameraManager : public Singleton<CameraManager> {
 public:
  //   friend class Singleton<CameraManager>;

  template <typename... Args>
  Camera* CreateCamera(Args... args) {
    auto camera = new Camera(args...);
    if (!m_defaultCamera) {
      m_defaultCamera = camera;
    }
    return camera;
  }
  Camera* GetDefaultCamera() { return m_defaultCamera; }

 protected:
  Camera* m_defaultCamera = nullptr;
};

}  // namespace Engine
}  // namespace WhiteDeer