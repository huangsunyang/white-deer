#pragma once

#include <utils/common/singleton.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace WhiteDeer {
namespace Engine {
class Camera {
 public:
  glm::vec3 GetZDirection() {
    auto dir = m_pos - m_target;
    if (dir.length == 0) {
      return glm::vec3(0, 0, -1);
    }
    return glm::normalize(dir);
  }

  glm::vec3 GetYDirection() {
    return glm::normalize(glm::cross(GetZDirection(), GetXDirection()));
  }

  glm::vec3 GetXDirection() {
    return glm::normalize(glm::cross(glm::vec3(0, 1, 0), GetZDirection()));
  }

  glm::mat4 GetViewMatrix() {
    return glm::lookAtRH(m_pos, m_target, glm::vec3(0, 1, 0));
  }

  glm::mat4 GetProjectionMatrix() {
    return glm::perspective(glm::radians(m_fov), 1920.0f / 1080.0f, 0.1f,
                            100.0f);
  }

  void SetPos(glm::vec3 p) {m_pos = p;}
  void SetTargetPos(glm::vec3 p) {m_target = p;}
  glm::vec3& GetPos() { return m_pos; }
  glm::vec3& GetTargetPos() { return m_target; }

 public:
  glm::vec3 m_pos = {0, 0, 1};
  glm::vec3 m_target = {0, 0, 0};
  float m_fov = 45.0f;

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