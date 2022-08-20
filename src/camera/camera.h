#pragma once

#include <utils/common/singleton.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "graphics/renderloop/renderloop.h"

namespace WhiteDeer {
namespace Engine {

using std::vector;
using WhiteDeer::Utils::Singleton;

class Camera {
  friend class WhiteDeer::Graphics::RenderLoop;

 public:
  glm::vec3 GetZDirection();
  glm::vec3 GetYDirection();
  glm::vec3 GetXDirection();
  glm::mat4 GetViewMatrix();
  glm::mat4 GetProjectionMatrix();

  void SetPos(glm::vec3 p) { m_pos = p; }
  void SetAspect(float aspect) { m_aspect = aspect; }
  void SetTargetPos(glm::vec3 p) { m_dir = glm::normalize(p - m_pos); }
  glm::vec3& GetPos() { return m_pos; }
  glm::vec3 GetTargetPos() { return m_dir + m_pos; }

  void Move(float, float, float);
  void Rotate(float, float);

 protected:
  glm::vec3 m_pos = {0, 0, 1};
  glm::vec3 m_dir = {0, 0, -1};

  float m_fov = 45.0f;
  float m_aspect = 1920.0f / 1080.0f;

  float m_speed = 0.1f;
};

class CameraManager : public Singleton<CameraManager> {
 public:
  //   friend class Singleton<CameraManager>;

  template <typename... Args>
  Camera* CreateCamera(Args... args) {
    m_cameras.push_back(new Camera(args...));
    return m_cameras.back();
  }

  vector<Camera*> GetCameras() { return m_cameras; }

  Camera* GetDefaultCamera() {
    return m_cameras.size() > 0 ? m_cameras[0] : nullptr;
  }

 protected:
  vector<Camera*> m_cameras;
};

}  // namespace Engine
}  // namespace WhiteDeer